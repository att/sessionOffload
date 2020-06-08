# How to contribute

Third-party contributions are essential for a successful open API definition.
Reference implementations will also help Application developers, NOS vendors and Merchant
Silicon providers to adhere to and create inter-operable solutions.

## Protocol Buffers vs Reference Implementations

New functionality is typically directed toward changes to the protocol buffers
with a corresponding change to the reference implementation to demonstrate how
it is used. The intent is to do basic validation of the protocol buffers with the
basic tests. Full blown error checking and regression would be done with the python simulator.
Other reference implementation may be added by the community to confirm or demonstrate
approaches such as with c++ or go clients and servers.


## Getting Started

*  Contributer License Agreement and Code Signing

This software is distributed under a permissive open source
license to allow it to be used in any projects, whether open
source or proprietary. Contributions to the project are welcome
and it is important to maintain clear record of contributions 
and terms under which they are licensed.

To indicate your acceptance of Developer's Certificate of Origin 1.1
terms, please add the following line to the end of the commit message
for each contribution you make to the project:

Signed-off-by : Your Name <your@email.com>


For information we have copied the Certificate of Origin 1.1 here:

```

Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or inpart by me and I
have the right to submit it under the open source license indicated
in the file: or

(b) The contribution is based upon previous work that, to the best
of my knowledge, is covered under an appropriate open source license
and I have the right under that license to submit that work with 
modifications, whether created in whole or part by me, under the same
open source license (unless I am permitted to submit under a different 
license), as indicated in the file; or

(c) The contribution was provided directly to me by some other person
who certified (a), (b) or (c) I have not modified it.

(d) I understand and agree that this project and the contribution are
public and that a record of the contribution (including all personal
information I submit with it, including my sign-off)is maintained 
indefinitely and may be redistributed consistent with this project or
the open source license(s) involved.

```


* Make sure you have a [GitHub account](https://github.com/join).
* Create an issue in the github repository issues tab for your issue if one does not already exist.
  * Clearly describe the issue including steps to reproduce when it is a bug.
  * Make sure you fill in the earliest version that you know has the issue.
* Fork the repository on GitHub.

## Making Changes

* Create a topic branch from where you want to base your work.
  * This is usually the master branch.
  * Only target release branches if you are certain your fix must be on that
    branch.
  * To quickly create a topic branch based on master, run `git checkout -b
    fix/master/my_contribution master`. Please avoid working directly on the
    `master` branch.
* Make commits of logical and atomic units.
* Check for unnecessary whitespace with `git diff --check` before committing.
* Make sure your commit messages are in the proper format. If the commit
  addresses an issue filed in the, start
  the first line of the commit with the issue number in parentheses.

* Make sure you have added the necessary tests for your changes.


## Making Trivial Changes

For trivial changes, it is not always necessary to create a new issue in github. 
In this case, it is appropriate to start the first line of a
commit with one of `(docs)`, `(maint)`, or `(packaging)` instead of a ticket
number.

If a issue exists for the documentation commit, you can include it
after the `(docs)` token.

```
    (docs)(DOCUMENT-000) Add docs commit example to CONTRIBUTING

    There is no example for contributing a documentation commit
    to the repository. This is a problem because the contributor
    is left to assume how a commit of this nature may appear.

    The first line is a real-life imperative statement with '(docs)' in
    place of what would have been the PUP project ticket number in a
    non-documentation related commit. The body describes the nature of
    the new documentation or comments added.
```

For commits that address trivial repository maintenance tasks or packaging
issues, start the first line of the commit with `(maint)` or `(packaging)`,
respectively.

## Submitting Changes

* Sign your code indicating agreement to the Contributer Code of Conduct 
In the future we may use a Contributer License Agreement as well.
* Push your changes to a topic branch in your fork of the repository.
* Submit a pull request to the github repository.
* Update the related issue to mark that you have submitted code and are ready
  for it to be reviewed.
* The core team looks at pull requests on a regular basis.
* After feedback has been given we expect responses within two weeks. After two
  weeks we may close the pull request if it isn't showing any activity.

## Revert Policy

By running tests in advance and by engaging with peer review for prospective
changes, your contributions have a high probability of becoming long lived
parts of the the project. 

If the code change results in a test failure, we will make our best effort to
correct the error. If a fix cannot be determined and committed within 24 hours
of its discovery, the commit(s) responsible _may_ be reverted, at the
discretion of the committer and maintainers. This action would be taken
to help maintain passing states in our testing pipelines.


### Summary

* Changes resulting in test pipeline failures will be reverted if they cannot
  be resolved within one business day.

## Additional Resources

* [General GitHub documentation](https://help.github.com/)
* [GitHub pull request documentation](https://help.github.com/articles/creating-a-pull-request/)
