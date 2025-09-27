# Cloth Sim
This is an implementation of X. Provot's 1995 paper describing a deformation constraint method for mass-spring model cloth simulations. The final report pdf provides an analysis of the implementation.

Pressing the space bar will toggle the deformation constraint off, which will likely make the simulation explode.

## Update (2025-09-25)
I wanted to try to improve the performance of the simulation by removing the spring instances and just iterating over the particles in pairs as the springs would have connected them. My thinking was that this would reduce the amount of data that would need to be loaded from memory, since the rest length and stiffness was the same for every instance of a specific spring type. However, this actually increased the simulation time by about 0.05ms on my computer, going from 0.15ms to 0.2ms per frame. My guess is iterating through the particles is less predictable than iterating through the springs, so the CPU's branch predictor fails more often.

Uncomment the `#define NO_STORED_SPRINGS` directive in Cloth.h to try the simulation without storing instances of the spring class. The console prints the average frame time over the last 60 frames.

I'd say this is further proof that you should always benchmark changes you make for performance reasons.

## Update (2025-09-26)
I was able to improve the performance of the simulation by more efficiently calculating the vertex normals. When I originally wrote this simulator, I naively calculated the normals per face, and then made the normal of the vertices an average of the faces they were a part of. Now I just calculate the normals based on the positions of the adjacent vertices. This brought the simulation time down by about 0.06ms on my computer, going from 0.15ms to 0.09ms per frame.

Somewhat interestingly, the new normal calculation is efficient enough that the overhead from running the calculations in parallel outweighs the benefit by about 0.01ms per frame. This would probably be different if the cloth was a higher resolution, but I haven't tried it.

Commenting out the `#define VERTEX_BASED_NORMALS` directive in Cloth.h will revert the simulation back to my old naive approach.