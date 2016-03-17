
# Plan:

- [x] Comment style
- [x] Refactor input
- [x] Component-based Design
- [x] Texturing
  - [x] Static
  - [x] Dynamic (moving texture)
- [x] Considers if use PIMPL universally
  No, PIMPL requires hand-made copy C'tor/operator, kind of pain. 
- [ ] Blending
- [ ] ...
- [ ] AI
- [ ] UI
- [ ] Audio
- [ ] Physics
- [ ] ...


## Blending
（3-16 - 3-20）

- [x] Read book. (3-16 - 3-17) 
- [ ] Implement. (3-18 - 3-20)
  - [x] Set blend state
  - [ ] Sort draw order
  - [ ] Add Fog


# Unplaned:

- [ ] Mouse bug: when mouse beyonds window's scope then release, no OS keyup message received.
- [ ] Keyboard bug: Presses both A and D, horizontal axis is positive.
- [ ] Moves resize from Window to Input
- [ ] Right hand coordinate system
- [ ] Learn LINQ

## Right hand coordinate system

- [ ] Model indices
- [ ] Calculations inside Camera