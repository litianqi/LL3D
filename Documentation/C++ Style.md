# C++ Features

## Namespace

A library has its global namespace, if this library is large enough to have multiple modules, each module could (and suggest) has its own sub namespace.

## RTTI

In use.

## Exception

In use. For details on error handling, see Architecture Specification


# Naming

* Namespace: FooBar

* File: FooBar

* Class: FooBar

* Enum: FooBar

* Macro: FOO_BAR

* Constant:
  * Global/Class: FooBar
  * Local: foo_bar

* Variable
  * Global: g_foo_bar
  * Static: s_foo_bar
  * Data Member: foo_bar_
  * Local: foo_bar

* Function: FooBar

# Formatting

  We use Visual Studio 2015 default format style. It's nice! :]
  // TODO: space

## Comment:

1. Section:

//////////////////////////////////////////////////////////////////////////
/////////////////////////////// Section

2. Summary:

///
// Summary...
//


# Other

* Order: 

  Let more important stuff be more front. Following are two examples using this principle.
  * public > protected > private
  * In public/protected, data > functions; Otherwise function > data.

* Restricting Access. 

  If you need only read it, then do not read+write it. This means we prefer access functions (getter) to directly access data member, and params/locals to data member. Data member is one of sources of mess, for we can read+write it, and access it in whole class (like a global in class scope), we should restrict our usage.

* Good Abstraction.

  1. Use PIMPL idom, do not write write implementation in header files (not a simple return statement). Even template functions should seperate its declaration and implementation.
