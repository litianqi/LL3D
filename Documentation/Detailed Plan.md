
# Plan:

- [x] Comment style
- [x] Refactor input
- [x] Component-based Design
- [x] Texturing
  - [x] Static
  - [x] Dynamic (moving texture)
- [x] Considers if use PIMPL universally
  No, PIMPL requires hand-made copy C'tor/operator, kind of pain. 
- [x] Blending
- [ ] Stenciling
- [ ] ...
- [ ] AI
- [ ] UI
- [ ] Audio
- [ ] Physics
- [ ] ...


# Unplaned:

- [ ] [Refactor] Remvoe GameObject::Start, Add GameObject::Iterator 
- [ ] [Bug] When mouse beyonds window's scope then release, no OS keyup message received.
- [ ] [Bug] Presses both A and D, horizontal axis is positive.
- [ ] Moves resize from Window to Input
- [ ] Right hand coordinate system
- [ ] Learn LINQ

## Right hand coordinate system

- [ ] Model indices
- [ ] Calculations inside Camera