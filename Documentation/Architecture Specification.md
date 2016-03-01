# General

## Unicode

We follow this guidline(the highest anwser):
http://programmers.stackexchange.com/questions/102205/should-utf-16-be-considered-harmful

## Internationl

Like [Unicode] section said, we use utf-8 for supporting international. 
Other technologies are not employed, for this project is still tiny and no needs to display a lot of text.

## Portability

Windows 10 x64 only.

## Concurrency

// TODO

## Error Handling

* For error from user side(wrong arguments. etc.): Use ASSERT, so user will know it.
* For expected error: Throw a exception.
  * For D3D error: Call ThrowIfFailed().
* For unexpected error(hardware interruption. etc.): Use ASSERT, so we can get a dump and debug it.

# PIMPL

We use this idiom everywhere.


# Game Specific

## DirectXMath

For vector/position/color/matrix. We do not use storage class, we use SSE type only. 
This way will make our development easier, And still function properly on x64 system.

## Coordinate System

We use left hand coordinate system:

![Coor Image 1](Coordinate System 1.jpg)
![Coor Image 2](Coordinate System 2.jpg)