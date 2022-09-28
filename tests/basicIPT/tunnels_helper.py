import socket
import sys
from typing import NamedTuple
from collections.abc import Iterable
import random

import tunneloffload_pb2

class Match():
    def __init__(self, 
                 source_ip=None,
                 source_ip_mask=32,
                 dest_ip=None,
                 dest_ip_mask=32,
                 tunnel_id=None,
                 spi=None,
                 geneve_vni=None):
        self.source_ip = source_ip
        self.source_ip_mask = source_ip_mask
        self.dest_ip = dest_ip
        self.dest_ip_mask = dest_ip_mask
        self.tunnel_id = tunnel_id
        self.spi = spi
        self.geneve_vni = geneve_vni


def random_key():
    val =  "%064x" % random.randrange(2**256)
    return val.encode()

def ipv4_to_int(ipv4):
    return int.from_bytes(socket.inet_pton(socket.AF_INET, ipv4), byteorder=sys.byteorder)


def assign_match_to_object(match_criteria, match: Match):
    # Assigning match object to match proto
    if match.source_ip:
        match_criteria.ipv4_match.source_ip = ipv4_to_int(match.source_ip)
        match_criteria.ipv4_match.source_ip_prefix = match.source_ip_mask

    if match.dest_ip:
        match_criteria.ipv4_match.source_ip = ipv4_to_int(match.dest_ip)
        match_criteria.ipv4_match.source_ip_prefix = match.dest_ip_mask

    
    if match.tunnel_id:
        match_criteria.tunnel_id = match.tunnel_id
        
    if match.spi:
        match_criteria.ipsec_match.spi = match.spi

    if match.geneve_vni:
        match_criteria.geneve_match.vni = match.geneve_vni


def create_ipsec_enc_tunnel(tunnelid,
                            match: Match,
                            spi, 
                            tunnel_type,
                            tunnel_source_ip, 
                            tunnel_destination_ip, 
                            enc_type=tunneloffload_pb2._AES256GCM64, 
                            next_action=tunneloffload_pb2.RECIRCULATE):

    ipsec_enc_tunnel = tunneloffload_pb2.IpTunnelRequest()
    ipsec_enc_tunnel.operation = tunneloffload_pb2._CREATE

    # Tunnel ID, next action adn match
    assign_match_to_object(ipsec_enc_tunnel.match_criteria, match)
    ipsec_enc_tunnel.tunnel_id = tunnelid
    ipsec_enc_tunnel.next_action = next_action

    # Defining IPSec Encryption
    ipsec_params = ipsec_enc_tunnel.ipsec_tunnel.ipsec_enc
    ipsec_params.ipsec_sa.spi = spi
    ipsec_params.ipsec_sa.encryption_key = random_key()
    ipsec_params.tunnel_type = tunnel_type
    ipsec_params.encryption_type = enc_type
    
    ipsec_params.ipv4_tunnel.source_ip = ipv4_to_int(tunnel_source_ip)
    ipsec_params.ipv4_tunnel.destination_ip = ipv4_to_int(tunnel_destination_ip)
    return ipsec_enc_tunnel

def update_ipsec_enc_tunnel(tunnel_id, 
                            spi):

    ipsec_enc_tunnel = tunneloffload_pb2.IpTunnelRequest()
    ipsec_enc_tunnel.tunnel_id = tunnel_id
    ipsec_enc_tunnel.operation = tunneloffload_pb2._UPDATE
    ipsec_params = ipsec_enc_tunnel.ipsec_tunnel.ipsec_enc
    ipsec_params.ipsec_sa.spi = spi
    ipsec_params.ipsec_sa.encryption_key = random_key()

    return ipsec_enc_tunnel

def update_ipsec_dec_tunnel(tunnel_id,
                            first_tunnel_spi=None,
                            first_tunnel_key=None,
                            first_tunnel_operation=None,
                            second_tunnel_spi=None,
                            second_tunnel_key=None,
                            second_tunnel_operation=None,
                            third_tunnel_spi=None,
                            third_tunnel_key=None,
                            third_tunnel_operation=None):

    ipsec_dec_tunnel = tunneloffload_pb2.IpTunnelRequest()
    ipsec_dec_tunnel.tunnel_id = tunnel_id
    ipsec_dec_tunnel.operation = tunneloffload_pb2._UPDATE

    ipsec_params = ipsec_dec_tunnel.ipsec_tunnel.ipsec_dec

    security_associations = []

    for spi, key, operation in [(first_tunnel_spi, first_tunnel_key, first_tunnel_operation), 
                                (second_tunnel_spi, second_tunnel_key, second_tunnel_operation), 
                                 (third_tunnel_spi, third_tunnel_key, third_tunnel_operation)]:
        if spi is None:
            continue
        
        security_association = tunneloffload_pb2.IPSecSAParams()
        security_association.spi = spi
        if key is not None:
            security_association.encryption_key = key
        security_association.operation = operation
        security_associations.append(security_association)
        
    ipsec_params.ipsec_sas.extend(security_associations)

    return ipsec_dec_tunnel


def create_ipsec_dec_tunnel(tunnelid, 
                            match: Match,
                            spi,
                            key, 
                            tunnel_type,
                            enc_type=tunneloffload_pb2._AES256GCM64,
                            next_action=tunneloffload_pb2.RECIRCULATE):

    ipsec_dec_tunnel = tunneloffload_pb2.IpTunnelRequest()
    ipsec_dec_tunnel.operation = tunneloffload_pb2._CREATE

    # Tunnel ID, next action adn match
    assign_match_to_object(ipsec_dec_tunnel.match_criteria, match)

    ipsec_dec_tunnel.tunnel_id = tunnelid
    ipsec_dec_tunnel.next_action = next_action

    # Defining IPSec Decryption
    ipsec_params = ipsec_dec_tunnel.ipsec_tunnel.ipsec_dec
    ipsec_params.tunnel_type = tunnel_type
    ipsec_params.encryption_type = enc_type

    security_association = tunneloffload_pb2.IPSecSAParams()
    security_association.spi = spi
    security_association.encryption_key = key
    security_association.operation = tunneloffload_pb2._CREATE
    ipsec_params.ipsec_sas.extend([security_association])

    return ipsec_dec_tunnel

def create_geneve_encap(tunnelid,
                        match,
                        geneve_source_ip,
                        geneve_dest_ip,
                        geneve_source_mac,
                        geneve_dest_mac,
                        geneve_vni,
                        next_action=tunneloffload_pb2.RECIRCULATE):

    geneve_encap_tunnel = tunneloffload_pb2.IpTunnelRequest()
    geneve_encap_tunnel.operation = tunneloffload_pb2._CREATE
    geneve_encap_tunnel.tunnel_id=tunnelid
    geneve_encap_tunnel.next_action = next_action
    # Assigning the match criteria
    # Tunnel ID, next action adn match
    assign_match_to_object(geneve_encap_tunnel.match_criteria, match)

    # Assigning a GENEVE ENCAPSULATION
    # This geneve encapsulation is without any options
    geneve_encap = geneve_encap_tunnel.geneve.geneve_encap
    geneve_encap.outer_ipv4_pair.source_ip = ipv4_to_int(geneve_source_ip)
    geneve_encap.outer_ipv4_pair.destination_ip = ipv4_to_int(geneve_dest_ip)
    geneve_encap.inner_mac_pair.source_mac = geneve_source_mac.encode()
    geneve_encap.inner_mac_pair.destination_mac = geneve_dest_mac.encode()
    geneve_encap.vni = geneve_vni

    # Example option
    geneve_option = tunneloffload_pb2.GeneveOption()
    geneve_option.option_class = int("0104", 16)
    geneve_option.type = int('11', 16)
    geneve_option.data = bytes.fromhex('10203040')
    geneve_option.length = int(len(geneve_option.data) / 4)
    geneve_encap.geneve_option.extend([geneve_option])
    geneve_encap.option_length = 4 #  8 bytes fixed tunnel header, 
    # 4 bytes option header, 4 bytes option data
    # optionlength is multiple of 4

    return geneve_encap_tunnel

def create_geneve_decap(tunnelid,
                        match,
                        next_action=tunneloffload_pb2.RECIRCULATE):

    geneve_decap_tunnel=tunneloffload_pb2.IpTunnelRequest()

    geneve_decap_tunnel.operation = tunneloffload_pb2._CREATE

    # Tunnel ID, next action adn match
    assign_match_to_object(geneve_decap_tunnel.match_criteria, match)

    # Assigning the match criteria
    geneve_decap_tunnel.tunnel_id=tunnelid
    geneve_decap_tunnel.next_action = next_action

    # Assigning a GENEVE Decapsulation
    # SetInParent is used for detecting that is is the operation to perform
    geneve_decap_tunnel.geneve.geneve_decap.SetInParent()

    return geneve_decap_tunnel
