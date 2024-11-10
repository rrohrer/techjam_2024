# techjam_2024

Game in C for the Tech gamejam techjam_2024

## Dependencies:
- `sdl2`
- `emscripten` (web build only)

## ToDo
### 11/5
- ~~clean out `graphics_context_new`~~
- set up cube primitive
- ~~scale cube to 1.0~~
- ~~render cube per set voxel~~
- ~~n dot l lighting~~
- ~~color pallet~~
- color variation in shader
- ~~vertex fog~~
- video of camera orbiting simple voxel patch

only mat functions verified:
- lookAt
- perpective
- mul

Everthing is right handed column major


lighting:
- ~~pass multiple matrices into v shader to get frag pos (pc and m)~~
- ~~pass ambient dir and light dir into shader~~
- ~~calculate light in shader~~

### 11/9
- line drawing for editor
- editor
    - simple mouse input
    - simple keyboard inpud
    - 3d picking
    - ~~line drawing~~
    - layer select
    - set element
    - place collider
    - place enemy / spawn
    - place player

