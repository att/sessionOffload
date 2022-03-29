import socket
import sys
from typing import NamedTuple

import tunneloffload_pb2

class Match(NamedTuple):
    source_ip: str = None
    source_ip_mask: int = 32
    dest_ip: str =  None
    dest_ip_mask: int = 32
    tunnel_id : int = None
    spi: int = None
    geneve_vni: int = None


def ipv4_to_int(ipv4):
    return int.from_bytes(socket.inet_pton(socket.AF_INET, ipv4), byteorder=sys.byteorder)


def assign_match_to_object(match_criteria, match: Match):
    # Assigning match object to match proto
    if match.source_ip:
        match_criteria.ipv4Match.sourceIp = ipv4_to_int(match.source_ip)
        match_criteria.ipv4Match.sourceIpPrefix = match.source_ip_mask

    if match.dest_ip:
        match_criteria.ipv4Match.sourceIp = ipv4_to_int(match.dest_ip)
        match_criteria.ipv4Match.sourceIpPrefix = match.dest_ip_mask

    
    if match.tunnel_id:
        match_criteria.tunnelId = match.tunnel_id

    if match.spi:
        match_criteria.ipsecMatch.spi = match.spi

    if match.geneve_vni:
        match_criteria.geneveMatch.vni = match.geneve_vni


def create_ipsec_enc_tunnel(tunnelid,
                            match: Match,
                            spi, 
                            tunnel_type,
                            tunnel_source_ip, 
                            tunnel_destination_ip, 
                            enc_type=tunneloffload_pb2._AES256GCM8, 
                            next_action=tunneloffload_pb2.RECIRCULATE):

    ipsec_enc_tunnel = tunneloffload_pb2.ipTunnelRequest()

    # Tunnel ID, next action adn match
    assign_match_to_object(ipsec_enc_tunnel.match_criteria, match)
    ipsec_enc_tunnel.tunnelId = tunnelid
    ipsec_enc_tunnel.nextAction = next_action


    # Defining IPSec Encryption
    ipsec_params = ipsec_enc_tunnel.ipsecTunnel.ipsecEnc
    ipsec_params.SPI = spi
    ipsec_params.tunnelType = tunnel_type
    ipsec_params.encryptionType = enc_type
    ipsec_params.encryptionKey = b'f9cb6358b98ebdd029142048bdee473ac72bc1fad0325d61f68f85bcb06bb602' # 256 bit 
    ipsec_params.ipv4_tunnel.sourceIp = ipv4_to_int(tunnel_source_ip)
    ipsec_params.ipv4_tunnel.destinationIp = ipv4_to_int(tunnel_destination_ip)
    return ipsec_enc_tunnel

def create_ipsec_dec_tunnel(tunnelid, 
                            match: Match,
                            tunnel_type,
                            enc_type=tunneloffload_pb2._AES256GCM8,
                            next_action=tunneloffload_pb2.RECIRCULATE):

    ipsec_dec_tunnel = tunneloffload_pb2.ipTunnelRequest()

    # Tunnel ID, next action adn match
    assign_match_to_object(ipsec_dec_tunnel.match_criteria, match)

    ipsec_dec_tunnel.tunnelId = tunnelid
    ipsec_dec_tunnel.nextAction = next_action

    # Defining IPSec Decryption
    ipsec_params = ipsec_dec_tunnel.ipsecTunnel.ipsecDec
    ipsec_params.tunnelType = tunnel_type
    ipsec_params.encryptionType = enc_type
    ipsec_params.encryptionKey = b'f9cb6358b98ebdd029142048bdee473ac72bc1fad0325d61f68f85bcb06bb602' # 256 bit 

    return ipsec_dec_tunnel

def create_geneve_encap(tunnelid,
                        match,
                        geneve_source_ip,
                        geneve_dest_ip,
                        geneve_source_mac,
                        geneve_dest_mac,
                        geneve_vni,
                        next_action=tunneloffload_pb2.RECIRCULATE):

    geneve_encap_tunnel = tunneloffload_pb2.ipTunnelRequest()
    geneve_encap_tunnel.tunnelId=tunnelid
    geneve_encap_tunnel.nextAction = next_action
    # Assigning the match criteria
    # Tunnel ID, next action adn match
    assign_match_to_object(geneve_encap_tunnel.match_criteria, match)

    # Assigning a GENEVE ENCAPSULATION
    # This geneve encapsulation is without any options
    geneve_encap = geneve_encap_tunnel.geneve.geneveEncap
    geneve_encap.outerIpv4Pair.sourceIp = ipv4_to_int(geneve_source_ip)
    geneve_encap.outerIpv4Pair.destinationIp = ipv4_to_int(geneve_dest_ip)
    geneve_encap.innerMacPair.sourceMac = geneve_source_mac.encode()
    geneve_encap.innerMacPair.destinationMac = geneve_dest_mac.encode()
    geneve_encap.vni = geneve_vni

    return geneve_encap_tunnel

def create_geneve_decap(tunnelid,
                        match,
                        next_action=tunneloffload_pb2.RECIRCULATE):

    geneve_decap_tunnel=tunneloffload_pb2.ipTunnelRequest()

    # Tunnel ID, next action adn match
    assign_match_to_object(geneve_decap_tunnel.match_criteria, match)

    # Assigning the match criteria
    geneve_decap_tunnel.tunnelId=tunnelid
    geneve_decap_tunnel.nextAction = next_action

    # Assigning a GENEVE Decapsulation
    # SetInParent is used for detecting that is is the operation to perform
    geneve_decap_tunnel.geneve.geneveDecap.SetInParent()

    return geneve_decap_tunnel