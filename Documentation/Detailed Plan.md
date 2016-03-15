
Plan:

- [x] Comment style
- [ ] Refactor: Input
- [x] Component-based Design:
- [ ] => Right hand coordinate system
- [ ] Texturing
- [ ] Considers if employ PIMPL
- [ ] Blending
- [ ] Learn LINQ
- ...


Refactor: Input:
(3-18 -  3-19)

- [x] Moves mouse/keyboard from Window to Input
- [ ] Moves resize from Window to Input


Component-based Design:
(3-? - 3-18)

- [x] Add Scene, GameObject, Component base class.
- [x] Convert Light/Camera/Model to Component
- [x] Moves transfrom from Model component to Transform component. 
- [x] Component order: 
      Currentlly Draw is placed in Model, so Model must be the last one to Update.
      Should Draw moved outof Model, or should Transform has a Draw too.

      A:
      STL is orderd by add sequence, as component is first added, there is no problem.


=> Right hand coordinate system