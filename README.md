# OpenColorIO bindings for Go

This package is a work-in-progress
----------------------------------

So far only bits and pieces of the API have been exposed. It's a starting point for anyone else that wants to jump in and contribute.

While the tests currently pass, there are still problems.

*TODO*

* The `config` struct wrapper is not safely maintaining the `OCIO::ConstConfigRcPtr` (a shared_ptr). The calls work fine within the same scope, but reusing the `config` across functions is a seg fault because the pointer is getting deleted. Need to figure out how to properly preserve it.
* Error Handling
* Implement all of the API


Requirements
----------------------

* [OpenColorIO](http://opencolorio.org/)


Installation
------------

    go get github.com/justinfx/opencolorigo
