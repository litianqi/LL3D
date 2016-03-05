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

- For errors presumably detectable before the program executes, such as violations of logical preconditions or class invariants:
  - If we want the error handling code only work in debug edition: use ASSERT.
  - Else: use std::logic_error hierarchy.(sparely )
- For errors presumably detectable only when the program executes: use std::runtime_error hierarchy.
- When you do not think there is an error, but you want check it anyway: use ASSERT.

## Singleton Problem

- Use Singleton only when this class will be accessed in whole application, for example logging/filesystem is, while graphics device is not(it only be used in graphics part of our application).
- Otherwise, use pass in.

## PIMPL

// TODO


# Game Specific

## DirectXMath

For vector/position/color/matrix. We do not use storage class, we use SSE type only. 
This way will make our development easier, And still function properly on x64 system.

## Coordinate System

We use left hand coordinate system:

![Left Hand Coor System](Coordinate System 1.jpg)
![Some Common Terms in Coor System](Coordinate System 2.jpg)