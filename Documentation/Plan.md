
# Plan:

- [x] [Refactor] Comment style
- [x] [Refactor] Input: Event-driven => Query API
- [x] [Refactor] Component-based design
- [x] [Feature] Add texturing
- [x] [Refactor] Considers if use PIMPL universally
- [x] [Feature] Add blending
- [ ] [Refactor] Model is composed with Faces; Face (1-1)references to Material; Material (1-1)references to Texture.
- [ ] [Feature] Add .obj .mtl files parser.
- [ ] [Feature] Add stenciling
- [ ] ...
- [ ] [Feature] Add AI
- [ ] [Feature] Add UI
- [ ] [Feature] Add audio
- [ ] [Feature] Add physics
- [ ] ...

## Stenciling
- [ ] Read book. (3-20 - 3-23)
- [ ] Implement (3-24 - 3-?)
  - [ ] Mirror 

# Unplaned:

- [ ] [Refactor] Remvoe GameObject::Start, add GameObject::Iterator 
- [ ] [Bugfix] When mouse beyonds window's scope then release, no OS keyup message received.
- [ ] [Bugfix] Presses both A and D, horizontal axis is positive.
- [ ] [Refactor] Moves resize from Window to Input
- [ ] [Refactor] Right hand coordinate system

## Right hand coordinate system

- [ ] Model indices
- [ ] Calculations inside Camera