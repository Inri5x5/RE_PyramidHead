# Assignment 3 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".
If you are answering a criteria in a different section, please describe that like: "4a. This criteria is met by the implementation described in 2b."

## Section 1: Design Pitch. Describe a possible setting and what kind of graphics technology would be necessary to implement this. These components do not have to be implemented in this project. Suggestion: 200 words should be more than enough for this.

The setting of this game was inspired from a old famous horror game "Silent Hill 3", where a certain character was stuck inside a hospital, and need to transverse to "Silent Hill World" by using a mirror to progress the story. However, in my game,player will be played as a 1st person instead of 3rd person, to focus more on solving the puzzle of the room.

The main technology used for this game would be reflectivity, it will have a lot of mirrors, where each reflect a different part of the other world. The other 2 technologies that was required but not implemented, were Collision detection ,such that the player wont go out of bound / see inside an object, and shadow mapping to increase the realistic horror experience.

P.S To move around from one world to the other, click "SPACE"

Assignmet was built on windows 10

## Section 2: Advanced Lighting

### 2a: Blinn-Phong Lighting. Include code references to the implementation of lighting in your project.

Bling Phong was implemented in every light sources on the game, which were the heart and light(lamp). You could see its effect when you (as a heart) 
get quite near to an object/mesh (door, floor). The main code would be inside `shaders/static_mesh.frag` line 80-83. which is used by a renderer to
render every static object on the scene `renderer/draw_model` line 104.

### 2b: High Dynamic Range. Show use of an intermediate frame buffer to store light values > 1.0 and how that is converted to Low Dynamic Range. Include code references.

HDR was implemented as a final pass of frmabuffer in my program. It was used after the kernel framebuffer applied its effect (`main` line 141-146).
Simply put, the kernel framebuffer will create a texture, which is passed to HDR framebuffer as a floating point such that it could hold color value > 1 (`framebuffer/make_HDRframebuffer` line 47). Then, that value will be converted back to low range inside `shaders/hdr.frag` line 19-22. 

## Section 3: Reflections

### 3a: Environment Mapped Reflections. Show at least one object can reflect a cube map. Cube map can be a preprepared skybox and the object doesn't need to reflect other objects in the scene. Include code references to implementation.

The object that used cubemap on the game would be a reflective heart in the middle of the "reality world", where its real-time reflecting its surrounding. Just like static cubemap, we assign it a texture generated as a cube map, then modify its tex coordinates inside `shaders/reflective.vert` line 15 - 16.

### 3b: Realtime Cube Map. Describe how the cube map is created during the frame render and then used for a reflective object. Include code references to the generation of the cube map (3a should already describe the reflection process).

You can see the real time cube map in the middle of "Reality World", where there is a reflective heart.
Since it is a real-time cube map, I used a different framebuffer, which will generate a cubemap texture inside (`framebuffer/make_CubeMapframebuffer` line 69), where I generarte 6 textures for each faces. Then using a hard-coded placed camera (`main` line 55), and after binding the framebuffer, it switches over to 6 different side and render the scene as 6 different textures. (`main` line 90). Finally, the texture 
was passed to the object asa complete cubemap texture (`main` line 59).

### 3c: In-World Camera/Magic Mirror. Describe how you have placed a camera in a scene that renders to a texture. Show where both of these objects are and how they function to show a different viewpoint of the scene. Include references to code.

Wiil be decribe more on 3d.

### 3d: Planar Reflections. Describe the system used to generate a planar reflection, including any techniques for perspective and/or near plane correction. Include references to code.

I used planar refelection as the main point of the game, where the player could see its reflection in the other world. The idea of the system would be to create a 2nd camera behind a mirror, reflect the position and direction (in this case camera's yaw) (fixing its perspective view) , modify the clipplane such that it did not render the back wall (near-plane correction), and bind-render it to a frame buffer which is passed in as a texture (`main` line 99-122), with its own shaders `shaders/mirror.vert`.

## Section 4: Post Processing

### 4a: Simple Colour Change. Describe how this project uses a framebuffer and what change is made to the colour data, including references to where this is implemented in code.

Will be explained more in 2b and 4b.

### 4b: Use of Kernels. Describe the kernel sampling technique in use, including code references.

The kernels/effect used for this game would be the sharpening effect, which could be seen in "Silent Hill World". The binding+rendering will take place on `main` line 143. The framebuffer was the same as the original framebuffer. The main part would be on `shaders/scary.frag`, where it sharpens
every pixel by sampling its surrounding pixel.
### 4c: Multiple Intermediate Framebuffers. Show use of more than one framebuffer for post processing (like chaining two post processing effects in a row). Include references to code.

The multiple framebuffer was used in "Silent Hill World", I used 2 intermediate framebuffer, which were the kernnel and HDR. Simply put, this is the way it renders
render as a scene -> kernel framebuffer -> render as a quad -> hdr framebuffer -> render as a quad -> screen (`main` line 134 -147)

### 4d: Multiple Render Targets. Describe how this project uses multiple frame buffers to write information from the initial fragment shader pass and how it uses them in post processing. Include references to fragment shader output code as well as post processing code.

I attempted this part, but unsuccefully debug the problem. I have succesfully render the mirror and reflective, but not the other. It seems it did not load the texture data correctly since if you go out of the room, the things inside will disappear. You could see the failed attempt by `main` line 130. 

The system that i have created was to create a new framebuffer or gbuffer to hold the position, normal and color. (`framebuffer/makeg_framebuffer`), 
then render the scene with different shaders (I created new shaders for different objects since they ned to pass in the data from frag. for example g_static_mesh, g_mirror, g_reflective), finally add lightning effect on `renderer/grender_lightning`. The main chain of function calls would be on the `main` line 124-132

### 4e: Temporal Post Processing. Show storage of framebuffers from previous frame(s) and how they are being used in post processing. Include references to where this is happening in code.

I did not complete part 4e. (If you would like your 4e to be marked, you will need to replace this line)

## Section 5: Show Me What You Got. Implement any techniques that are not part of previous criteria for assignments in this course. This is open, so must be documented clearly with code references to show what has been created to add to the visual effects of the scene or objects in it. 

For deffered shading, it has beed explained in 4d.

## Section 6: Subjective Mark

### How have you fulfilled this requirement?

Eventhough it has cost many trials and errors, I manage to create the least expected of my vision to create a horror-mystery room game. Especially with how the reflections, the sharpening effect and blinking light, really enchance the experience. However, I am not able to make it more scarier such as making the pyramid head attack the player or play a dark backgroud music or simlpy adding some shadows.

### What is your vision for the scene you are showing?

My vision of this game would be to replicate what "Silent Hill 3" did in old times, where player can see the dark side of the other world, while totatlly reflecting whats in reality.

### Are you referencing anything in this scene? Is this a quote or homage to any other art? Please describe it.

Yes, Silent Hill3 was the main references ( Mainly this part https://www.youtube.com/watch?v=tkYtflr1KsM)

### Please give instructions for how a viewer/player should interact with this scene to see all the content you have created.

You can move around between world by pressing space, and move through out the room. However, try not to move inside an object since there isnt any collsiion detection ont his assignment.

## Section 7 (optional): Code Style Feedback

### There are no marks assigned to code style in this assignment. However, if you would like to receive some feedback on your coding style, please point out a section of your code (up to 100 lines) and your tutor will give you some written feedback on the structure and readability of that code.

Write your answer here (lines numbers or function names that you'd like reviewed)

## Section 8 (optional): Use of External Libraries

### What does the library do?

Write your answer here...

### Why did you decide to use it?

Write your answer here...

### How does this integrate with the assignment project build system? Please include any instructions and requirements if a marker needs to integrate an external library.

Write your answer here...
