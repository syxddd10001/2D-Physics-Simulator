// GLSL version directive
#version 120 // Must match the vertex shader version

// Inputs from the vertex shader (these match the 'varying' outputs from the vertex shader)
varying vec4 v_Color;          // Interpolated vertex color (replaces gl_FrontColor)
varying vec2 v_TexCoords;      // Interpolated texture coordinates (replaces gl_TexCoord[0].xy)

// Uniforms (same as before, these are custom for your glow effect)
uniform sampler2D u_texture;       // Optional: If your shape has a texture
uniform vec4 u_glowColor;          // The color of the glow (e.g., orange, blue)
uniform float u_glowStrength;      // How intense the glow is (e.g., 0.5 to 2.0)
uniform float u_radius;            // The actual radius of the circle being drawn
uniform float u_glowRadiusMultiplier; // How much larger the glow should be than the base object

// Output to the framebuffer (the final color of the pixel)
// This is still gl_FragColor for GLSL 120. In newer versions (330+ core) it would be 'out vec4 FragColor;'
// but for SFML's default context, gl_FragColor is fine.

void main()
{
    // v_TexCoords holds the interpolated texture coordinates for the current fragment.
    vec2 texCoords = v_TexCoords; // Renamed for clarity, but it's the varying input

    // Convert texture coordinates from [0,1] range to [-1,1] range, centered at (0,0).
    vec2 pos = texCoords * 2.0 - 1.0;

    // Calculate the distance from the center of the shape.
    float distFromCenter = length(pos);

    // Get the base color of the fragment from the interpolated vertex color.
    vec4 baseColor = v_Color; // Use the modern 'varying' input

    // If you're using a texture for your circle (e.g., a white circle image), uncomment this:
    // baseColor *= texture2D(u_texture, texCoords);


    // --- Glow Calculation Logic ---
    float falloff = 1.0 - smoothstep(0.0, 1.0, distFromCenter);
    falloff = pow(falloff, u_glowRadiusMultiplier * 2.0);

    float glowFactor = falloff * u_glowStrength;
    glowFactor = clamp(glowFactor, 0.0, 1.0);


    // --- Final Color Blending ---
    // The temporary debug change from previous step is removed here.
    // If glowFactor is very low (center of object), mix with baseColor,
    // otherwise mix with glowColor.
    vec4 finalColor = mix(baseColor, u_glowColor, glowFactor);

    // Set the output alpha based on the glow factor. This helps with the blending.
    finalColor.a = glowFactor * u_glowColor.a;

    gl_FragColor = finalColor; // Output the final pixel color
}