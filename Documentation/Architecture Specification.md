# General

## Unicode

We follow this guidline(the highest anwser):
http://programmers.stackexchange.com/questions/102205/should-utf-16-be-considered-harmful

## Internationl

Like [Unicode] section said, we use utf-8 for international. 
Other technologies are not employed, for this project is still tiny and no needs to display a lot of text.

## Portability

Windows 10 x64 only.

## Concurrency

// TODO

## Error Handling

- Logic Error

Errors that are a consequence of faulty logic within the program such as violating logical preconditions or class invariants and may be preventable.

If you want the client side handle it, or see it even in release version, use std::logic_error. Else (client cann't handle it, and only see it in debug version), use ASSERT.

- Domain Error

Errors that are due to events beyond the scope of the program and can not be easily predicted.

Use std::runtime_error.


## Singleton Problem

- Use Singleton only when this class will be accessed in whole application, for example logging/filesystem is, while graphics device is not(it only be used in graphics part of our application).
- Otherwise, use pass in.

## PIMPL

We will not use PIMPL universally, PIMPL requires hand-made copy C'tor/operator, kind of pain. 


# Game Specific

## DirectXMath

For vector/position/color/matrix. We do not use storage class, we use SSE type only. 
This way will make our development easier, And still function properly on x64 system.

## Coordinate System

We use left hand coordinate system:

![Left Hand Coor System](Coordinate System 1.jpg)
![Some Common Terms in Coor System](Coordinate System 2.jpg)