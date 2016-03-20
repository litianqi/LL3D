
## Refactor: Input:
(3-18 -  3-19)

- [x] Moves mouse/keyboard from Window to Input

## Component-based Design:
(3-? - 3-18)

- [x] Add Scene, GameObject, Component base class.
- [x] Convert Light/Camera/Model to Component
- [x] Moves transfrom from Model component to Transform component. 
- [x] Component order: 
	  
	  Problem:
      Currentlly Draw is placed in Model, so Model must be the last one to Update.
      Should Draw moved outof Model, or should Transform has a Draw too.

      Solution:
      STL is orderd by add sequence, as component is first added, there is no problem.

## Blending
（3-16 - 3-20）

- [x] Read book. (3-16 - 3-17) 
- [x] Implement. (3-18 - 3-20)
  - [x] RecursiveSceneIterator
  - [x] Sort draw order
  - [x] Camera
  - [x] Component: Add Start()
  - [x] Add Fog 