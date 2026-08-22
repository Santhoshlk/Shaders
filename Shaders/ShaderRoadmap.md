# Shaders — Zero to Competent in 4 Weeks

**2.5 hrs/day · pure GLSL · Shadertoy · no engine, no framework, no boilerplate**

Every day below has its links inline. Nothing needs looking up elsewhere.

---

## Contents

- [What 4 weeks buys](#what-4-weeks-buys)
- [Part 0 — Setup tonight (30 min)](#part-0--setup-tonight-30-min)
- [Part 1 — Day 0: the actual zero point](#part-1--day-0-the-actual-zero-point)
- [Part 2 — Debugging playbook](#part-2--debugging-playbook)
- [Part 3 — The stuck protocol](#part-3--the-stuck-protocol)
- [Week 1 — Think in pixels](#week-1--think-in-pixels-days-17)
- [Week 2 — Noise and procedural texture](#week-2--noise-and-procedural-texture-days-814)
- [Week 3 — Raymarching and 3D SDFs](#week-3--raymarching-and-3d-sdfs-days-1521)
- [Week 4 — Make something that's yours](#week-4--make-something-thats-yours-days-2228)
- [Part 4 — Slack rules](#part-4--slack-rules)
- [Part 5 — Full resource index](#part-5--full-resource-index)
- [Part 6 — Why this order](#part-6--why-this-order)

---

## What 4 weeks buys

70 hours at full attendance. Assume ~55 effective after slack.

**You will be able to:** think in per-pixel parallel terms fluently · write 2D procedural shaders from a blank file · build noise and fBm from scratch · raymarch a 3D SDF scene with lighting, soft shadows and AO · open an intermediate Shadertoy shader, read it, and modify it deliberately · produce original pieces that aren't tutorial output.

**You will not be:** a graphics programmer yet. No rasterization pipeline, no PBR derivation, no GPU architecture, no profiling. That's the renderer track after this — this block exists so that track isn't fighting two unfamiliar things at once.

That's a real, nameable skill in 4 weeks. It's the tech-artist half of graphics, and it's the half that makes the other half readable.

---

# PART 0 — SETUP TONIGHT (30 MIN)

Free, one session, no installs. The point is that Day 1 opens with you typing code.

### 1. Shadertoy account — 5 min
https://www.shadertoy.com → sign up → create a shader → save it → confirm it appears on your profile.
Without an account, everything you write dies when the tab closes.

### 2. Bookmarks folder `shaders` — 5 min
| # | Link | Used for |
|---|---|---|
| 1 | https://thebookofshaders.com | weeks 1–2 spine |
| 2 | https://iquilezles.org/articles/distfunctions/ | week 3 onward, SDF list |
| 3 | https://shaderific.com/glsl-functions | built-in function lookup |
| 4 | your own Shadertoy profile | your work |

### 3. Local repo — 10 min
```
D:\C++Series\Shaders\
├── README.md
├── week1-basics\
├── week2-noise\
├── week3-raymarch\
├── week4-original\
└── reference\
    └── glsl-cheatsheet.md
```
`git init`, push it empty tonight.

End of each build day: paste the `.glsl` source into the right folder, Shadertoy link as a comment on line 1, commit. 30 seconds. Keeps the graph moving during a block where the real output lives on someone else's website.

README format: one line per shader — name, technique it demonstrates, link.

### 4. Reference card — 10 min
Copy the GLSL card from [Day 0](#12--glsl-reference-card) into `reference/glsl-cheatsheet.md`. Keep it open in a second tab all of week 1.

### Prerequisite check
| Needed | Where you have it |
|---|---|
| Vectors, dot product, normalize | Freya's math series |
| Trig, angles, polar coordinates | Freya's math series |
| Matrices — 2D rotation | Freya's math series |
| C-family syntax | obviously |
| A GPU that runs WebGL | any machine from the last decade |

**Not needed, do not go get first:** OpenGL, C++ graphics setup, any engine, linear algebra beyond the above.

---

# PART 1 — DAY 0: THE ACTUAL ZERO POINT

One session. Assumes you have never written a shader.

## 1.1 — Where a shader sits

A mesh's vertices go through a **vertex shader** (positions them), then **rasterization** turns triangles into pixel-sized fragments, then a **fragment shader** runs *once per fragment* and outputs a colour. That's the pipeline in one line.

The thing that breaks C++ habits: those fragment shader invocations run **simultaneously and independently**. There is no loop over pixels — you write the code for *one* pixel, and the GPU runs a few thousand copies at once. A pixel cannot see its neighbour, cannot write to a shared variable, cannot know what any other pixel decided. Every technique in this plan is a consequence of that constraint.

Shadertoy skips the vertex stage entirely: you get one fullscreen quad, so every pixel on screen runs your fragment shader. The whole pipeline reduces to the one stage you're learning.

## 1.2 — GLSL reference card

**Types**
`float` · `vec2` `vec3` `vec4` · `mat2` `mat3` `mat4` · `int` · `bool`
No classes, no pointers, no `new`, no recursion, no dynamic allocation. `float` is the default currency.

**Swizzling** — `v.xy` · `v.rgb` · `v.xxx` · `v.wz`. Reorder or duplicate components inline. `col.bgr` flips red and blue. Used constantly.

**Component-wise math** — `vec3 * vec3` multiplies element by element, **not** a dot product.

**Constructors** — `vec3(1.0)` fills all three. `vec4(rgb, 1.0)` composes from a `vec3` plus a scalar.

| Function | What it does | Where you'll use it |
|---|---|---|
| `mix(a, b, t)` | linear blend | every colour blend, every fade |
| `clamp(x, lo, hi)` | bound a value | keeping colours in 0..1 |
| `step(edge, x)` | 0 below, 1 above — hard | binary masks |
| `smoothstep(a, b, x)` | 0→1 with a soft ramp | **every soft edge you ever write** |
| `fract(x)` | fractional part | tiling, repetition, hashes |
| `floor(x)` | round down | cell index in a grid |
| `mod(x, y)` | remainder | domain repetition |
| `length(v)` | magnitude | distance to a point → circles |
| `distance(a, b)` | `length(a - b)` | same |
| `normalize(v)` | unit vector | ray directions, normals |
| `dot(a, b)` | projection | diffuse lighting, angles |
| `cross(a, b)` | perpendicular | camera bases |
| `atan(y, x)` | angle | polar coordinates |
| `abs` `sign` `min` `max` `pow` `exp` `sin` `cos` | scalar utility | everywhere |

These are hardware-fast. Reach for them instead of writing `if`.

**Branching is expensive.** Neighbouring pixels taking different `if` paths means the GPU runs both and throws one away. Prefer `step` / `smoothstep` / `mix` to express a choice as arithmetic — same reasoning as SIMD lanes.

**Floats need decimals.** `1` is an int; `1.0` is a float. `float x = 1;` is a compile error. Bites on day one, then never again.

## 1.3 — The Shadertoy entry point

```glsl
void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord / iResolution.xy;   // 0..1 across the screen
    fragColor = vec4(uv, 0.0, 1.0);
}
```

| Uniform | Meaning |
|---|---|
| `fragCoord` | this pixel's position **in pixels**, (0,0) bottom-left |
| `iResolution` | screen size in pixels |
| `iTime` | seconds since start |
| `iMouse` | `.xy` cursor position, `.zw` click position |
| `iFrame` | frame counter |

Type it, see the red-green gradient, and you've written a shader.

## 1.4 — The two UV setups

The single biggest early confusion, so it lives on day 0 rather than being discovered painfully in week 1.

```glsl
// A) 0..1 across the screen — gradients, tiling, texture-style work
vec2 uv = fragCoord / iResolution.xy;

// B) centred and aspect-corrected — origin at screen centre,
//    y runs -0.5..0.5, x scales with the window.
//    USE THIS ANY TIME YOU DRAW A SHAPE.
vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y;
```

With setup A, a circle drawn from `length(uv)` comes out an **ellipse** — one screen unit horizontally isn't one unit vertically. Dividing both components by `iResolution.y` makes the units square. Every shape shader on Shadertoy starts with some version of B.

If a shape ever looks stretched, this line is the first thing to check.

**Out of scope for four weeks:** vertex shaders, the OpenGL API side (compilation, uniform binding, VAOs/VBOs), anything engine-specific. Those come with LearnOpenGL and Ben Cook.

---

# PART 2 — DEBUGGING PLAYBOOK

Read before week 1, not after you get stuck. No printf, no breakpoints, no stack trace in a fragment shader. A black screen is the standard failure mode and this is the triage.

## 2.1 — Colour is your debugger
```glsl
fragColor = vec4(vec3(d), 1.0);              // a float as greyscale
fragColor = vec4(uv, 0.0, 1.0);              // a vec2 as red/green
fragColor = vec4(n * 0.5 + 0.5, 1.0);        // a normal (-1..1 → 0..1)
fragColor = vec4(vec3(steps) / 100.0, 1.0);  // raymarch step count
```
Grey where you expected shape → distance function wrong.
Pure black → value negative or zero everywhere.
Pure white → value ≥1 everywhere; scale it (`d * 0.1`) to see structure.

**When confused, output the intermediate value directly.** Don't reason about it, look at it.

## 2.2 — Black screen triage, in order
1. **Read the compile error.** Shadertoy prints it under the editor. Half of all black screens are a compile failure, not a logic bug.
2. **`1` vs `1.0`.** Most common compile error there is.
3. **Alpha.** `vec4(col, 1.0)` — a missing `1.0` gives a transparent image.
4. **Negative values.** Colours below 0 clamp to black. Output `abs(value)`.
5. **Scale.** Your shape may exist and be 0.001 units wide. Multiply UV by 0.1 to zoom out.
6. **Comment out from the bottom up.** Replace the last line with a flat colour, work backwards until something appears.

## 2.3 — Colour outside 0..1
Above 1 clamps white, below 0 clamps black. Your maths happily produces 4.7 and -2.3. A blown-out white blob usually means the shape is right and the range is wrong — `clamp`, scale, or `smoothstep` it into range.

## 2.4 — Infinite loops
A `while` with a condition that never trips hangs the tab or crashes the driver. Always a bounded `for (int i = 0; i < 100; i++)` with `break` inside. Never `while (true)`.

## 2.5 — When it's slow
Reduce the raymarch iteration count first, then the fBm octave count. Those two are 90% of the cost of anything in these four weeks.

---

# PART 3 — THE STUCK PROTOCOL

**20 minutes of genuine attempt. Then look.** Open the reference, the video, iq's page — whatever unsticks you. Read the line you were missing, understand why it's that, continue, finish the shader.

That is not cheating. The learning is in constructing the expression, and you still construct the remaining 90%. Grinding one line for two hours costs a session and teaches nothing.

**The one thing that stays hard:** every concept gets typed from a blank file at least once, even if you looked things up three times while doing it. Looking things up mid-build is fine forever. Copy-pasting a finished shader and moving on is the only thing that actually skips the learning.

**Two rules for the whole block:**
1. Every concept typed from a blank file at least once.
2. Save every shader publicly. Four weeks later that's a portfolio page, not a folder.

---

# WEEK 1 — THINK IN PIXELS (Days 1–7)

**Goal:** stop writing loops over objects. Start writing one expression that every pixel evaluates independently.

| Day | Material | Link |
|---|---|---|
| 1 | kishimisu — Intro to Shader Art Coding | [video](https://www.youtube.com/watch?v=f4s1h2YETNY) |
| 1 | Freya — Shaders for Game Devs Part 1 | [video](https://www.youtube.com/watch?v=kfM-yu0iQBk) |
| 2 | Freya — Shaders for Game Devs Part 2 | [video](https://www.youtube.com/watch?v=mL8U8tIiRRg) |
| 3–4 | Book of Shaders ch. 1–6 | [book](https://thebookofshaders.com) |
| 5–6 | Book of Shaders ch. 7–9 | [book](https://thebookofshaders.com) |
| 7 | Build — no tutorials | — |

---

### Day 1 — the per-pixel mental model, then the surface framing

**Watch first (~20 min):** kishimisu, *An Introduction to Shader Art Coding*
https://www.youtube.com/watch?v=f4s1h2YETNY
His Shadertoy profile: https://www.shadertoy.com/user/kishimisu

Watch once, close it, **rebuild his shader from a blank file.** You will fail partway — look, patch, continue. That one video installs the per-pixel mental model faster than anything else that exists, and it's Shadertoy-native, so it's the same environment you'll be in for four weeks.

**Then start (~2 hrs):** Freya Holmér, *Shaders for Game Devs Part 1 — Shader Basics, Blending & Textures*
https://www.youtube.com/watch?v=kfM-yu0iQBk
Playlist: https://www.youtube.com/playlist?list=PLImQaTpSAdsCnJon-Eir92SZMl7tPBS4Z
Course doc with assignments + assets: https://docs.google.com/document/d/1h_7O9n5-q8Kb-rhnMJyF4Tab6i3lH97E4Ykn7tsP-zc/edit

Demonstrated in Unity, content is engine-agnostic. **Watch for the concepts, don't install Unity.**

**Output:** kishimisu's shader rebuilt in your own Shadertoy tab, saved.

---

### Day 2 — SDFs and lighting, the surface-shader way

**Watch:** Freya, *Shaders for Game Devs Part 2 — Healthbars, SDFs & Lighting*
https://www.youtube.com/watch?v=mL8U8tIiRRg

What this gives you that Shadertoy structurally cannot: the *surface shader* framing, where geometry already exists and your job is to shade it. That's the framing LearnOpenGL and Ben Cook both use later, so having it early costs two days and saves months of confusion.

**Output:** notes only. Nothing to build yet.

---

### Days 3–4 — Book of Shaders, chapters 1–6

https://thebookofshaders.com

| Ch | Topic |
|---|---|
| 01 | What is a shader |
| 02 | Hello world |
| 03 | Uniforms — `u_time`, `u_resolution`, `u_mouse` |
| 04 | Running your shader |
| 05 | **Shaping functions** — `step`, `smoothstep`, `mix`, `clamp`, `fract`, `pow`, `sin` |
| 06 | Colour, `vec3` mixing, HSB→RGB |

**Naming note:** the book's editor uses `u_time` / `u_resolution` / `gl_FragColor`. Shadertoy uses `iTime` / `iResolution` / `fragColor`. Same concepts, different names — translate as you go, you stop noticing after a day.

`smoothstep` is the single most important function in this entire block. Every soft edge, gradient, mask and blend you write for the rest of your life is smoothstep. Spend real time on it.

**Output:** each shaping function tried in your own tab until you can predict its curve.

---

### Days 5–6 — Book of Shaders, chapters 7–9

| Ch | Topic |
|---|---|
| 07 | Shapes — 2D distance fields (circle, rect, polygon) |
| 08 | Matrices — 2D rotation, translation, scale |
| 09 | Patterns — `fract` tiling, grids, cell indices |

Polar coordinates land in ch. 07 — `atan(y, x)` for angle, `length` for radius. Freya already covered SDFs conceptually, so this goes faster than it looks.

**Output:** one tiled pattern of your own, saved.

---

### Day 7 — build, no tutorials

Pick two, from a blank file:
1. Animated gradient that responds to `iMouse`
2. A circle with a soft glow and a moving highlight
3. Tiled grid where each cell animates on an offset (`fract` + cell index)
4. Analog clock — hands from polar coordinates and `iTime`

Week 1 is the densest week in the plan. If it spills a day into week 2, let it.

> **Week 1 exit test:** draw a soft-edged circle at an arbitrary position without looking anything up. If yes, week 1 is done regardless of what you skipped.

---

# WEEK 2 — NOISE AND PROCEDURAL TEXTURE (Days 8–14)

**Goal:** stop drawing shapes, start generating surfaces. Highest-leverage week in the plan.

| Day | Material | Link |
|---|---|---|
| 8–9 | Book of Shaders ch. 10–11 | [book](https://thebookofshaders.com) |
| 10–11 | Book of Shaders ch. 12–13 | [book](https://thebookofshaders.com) |
| 12–14 | Build — no tutorials | — |

---

### Days 8–9 — randomness and noise

| Ch | Topic |
|---|---|
| 10 | Random — hash functions in GLSL, why `fract(sin(x)*43758.5453)` and why it's a hack |
| 11 | Noise — value noise, interpolation, gradient noise |

Why smoothstep matters here again: interpolating between random values with a linear ramp gives you visible creases. Smooth interpolation is what makes noise look like noise.

Optional context, one page: https://thebookofshaders.com/examples/

---

### Days 10–11 — cellular noise and fBm

| Ch | Topic |
|---|---|
| 12 | Cellular / Voronoi noise |
| 13 | **Fractal Brownian Motion** — stacked octaves |

fBm is the payoff. Once you have it, clouds, marble, wood, terrain, fire and water are all the same function with different parameters.

The shape to internalise:
```
amplitude = 0.5, frequency = 1.0
repeat N times:
    total     += amplitude * noise(uv * frequency)
    frequency *= 2.0     // each octave twice as fine
    amplitude *= 0.5     // and half as strong
```

Understand it rather than pasting it.

---

### Days 12–14 — build, no tutorials

1. Cloud layer — fBm, scrolling, soft threshold
2. Wood grain — fBm distorting a ring function
3. Fire — fBm with upward domain scroll and a heat colour ramp
4. Terrain heightmap rendered as a greyscale 2D image

> **Week 2 exit test:** write fBm from scratch, no reference, get something plausible. If you can, the second half of the plan is unlocked.

---

# WEEK 3 — RAYMARCHING AND 3D SDFs (Days 15–21)

**Goal:** invent 3D geometry inside a fragment shader with no mesh, no vertex data, nothing from the CPU.

This is the conceptual jump of the whole block, and the load-bearing week.

**The idea in one paragraph, so the video isn't your first contact with it:** an SDF is a function that takes a point in space and returns the distance to the nearest surface. So from any point you know how far you can travel safely without hitting anything — jump that far, evaluate again, repeat. When the distance gets tiny you've hit the surface; when you've travelled too far you've missed. That's the entire algorithm. Everything else is which SDF, and how you shade the hit.

| Day | Material | Link |
|---|---|---|
| 15 | Art of Code — Ray Marching for Dummies | [video](https://www.youtube.com/watch?v=PGtv-dBi2wE) |
| 16 | Art of Code — Simple Shapes | [video](https://www.youtube.com/watch?v=Ff0jJyyiVyw) |
| 17 | Art of Code — Basic Operators | [video](https://www.youtube.com/watch?v=AfKGMUDWfuE) |
| 18–19 | Lighting + Freya Part 3 | [video](https://www.youtube.com/watch?v=E4PHFnvMzFc) |
| 20–21 | Build | — |

---

### Day 15 — the raymarch loop

**Watch + follow along:** The Art of Code, *Ray Marching for Dummies*
https://www.youtube.com/watch?v=PGtv-dBi2wE

Live-coded start to finish. Type along in your own tab, don't just watch. Then rebuild it blank.

Optional companion read, genuinely gentle: https://blog.maximeheckel.com/posts/painting-with-math-a-gentle-study-of-raymarching/

---

### Day 16 — primitives

**Watch:** The Art of Code, *Simple Shapes*
https://www.youtube.com/watch?v=Ff0jJyyiVyw

- sphere, box, torus, capsule, plane SDFs
- step count, max distance, surface epsilon

**Bookmark permanently — the canonical SDF list:**
https://iquilezles.org/articles/distfunctions/

These are published, derived primitives. Using them directly is the norm, not a shortcut — understanding what each one does is the exercise.

---

### Day 17 — combining shapes

**Watch:** The Art of Code, *Basic Operators*
https://www.youtube.com/watch?v=AfKGMUDWfuE

- union / subtract / intersect (`min`, `max`, negation)
- **smooth-min** — the thing that makes SDF art look organic instead of CSG-boolean

iq on smin, the reference version: https://iquilezles.org/articles/smin/

Extra if you want it: Art of Code, *Tips & Tricks* — https://www.youtube.com/watch?v=Vmb7VGBVZJA

---

### Days 18–19 — lighting, and tangent space

**Build:**
- normals by gradient sampling (four or six taps around the hit point) — iq's method: https://iquilezles.org/articles/normalsSDF/
- Lambert diffuse, then Blinn-Phong specular
- soft shadows by marching toward the light
- ambient occlusion by sampling along the normal

You already know this math from Freya's math series — dot products for diffuse, reflection vectors, normals. This is where it stops being abstract.

**Also this evening:** Freya, *Shaders for Game Devs Part 3 — Normal Maps, Tangent Space & IBL*
https://www.youtube.com/watch?v=E4PHFnvMzFc

Shadertoy will never teach you tangent space, because it has no mesh and therefore no tangents. But the moment you touch a real geometry pipeline you need it, and this is the clearest explanation of it anywhere. Sits naturally next to the lighting work.

Alternative written take on SDFs if you want a second angle: https://mini.gmshaders.com/p/sdf · https://reindernijhoff.net/2017/07/raymarching-distance-fields/

---

### Days 20–21 — build

1. Lit sphere on an infinite plane, with a soft shadow
2. A shape sculpted from three primitives via smooth-min
3. Infinite field of objects via domain repetition (`mod`)
4. Moving camera through your own scene

> **Week 3 exit test:** raymarch a scene of your own design, lit, with a shadow, from a blank file. Hardest checkpoint in the plan and the one worth protecting.

---

# WEEK 4 — MAKE SOMETHING THAT'S YOURS (Days 22–28)

**Goal:** stop doing exercises. Produce work.

| Day | Task |
|---|---|
| 22–23 | Read and break Seascape |
| 24–26 | One original piece |
| 27–28 | Polish and publish |

---

### Days 22–23 — read a real shader

**Seascape:** https://www.shadertoy.com/view/Ms2SD1

~200 lines, well commented, and it is exactly your two weeks combined: it raymarches a *heightfield* driven by fBm, gets normals from the height function, then shades with sky colour, Fresnel and a fake subsurface term for foam.

Read it. Then break it deliberately — delete the Fresnel term, see what changes. Kill an octave. Change the tracing step. Then rebuild the heightfield-tracing function yourself from a blank file.

Breaking working code on purpose is the fastest way to find out what each line was actually doing.

Browse for more targets: https://iquilezles.org/articles/

---

### Days 24–26 — one original piece

Your own scene, combining fBm with raymarching. Not a copy of anything. Candidates:
- procedural terrain with distance fog
- volumetric clouds via density marching
- a lava surface
- a starfield with parallax layers

Pick one, make it good.

---

### Days 27–28 — polish and publish

- Clean the code, comment it properly (same professional bar you hold everywhere else)
- Publish your best 4–5 shaders on Shadertoy
- Push the repo with GLSL sources and the README of what each one demonstrates
- One LinkedIn post with a capture of the best piece

> **Block exit criterion:** an original raymarched piece built without a tutorial. That's the gate to Ben Cook.

---

# PART 4 — SLACK RULES

Four weeks will not be clean. Assume it isn't.

**Lose 1–3 days:** absorb them. Every week ends with 2–3 build days — those compress. Don't reschedule anything.

**Exams land:** switch to **30–45 min/day, reading only.** iq's articles, other people's Shadertoy source, no building. Zero-guilt mode — the point is staying in contact with the material, not progress. Resume the current week where you left off.

**Lose a full week:** cut in this order — (1) the four polish days, (2) two of the four build pieces per week, (3) cellular/Voronoi noise, (4) AO. Never cut fBm, never cut the raymarch loop, never cut the rebuild-from-blank steps.

**Hard floor:** even a wrecked week contains one shader typed from a blank file. That alone keeps the skill alive.

**What never moves:** week 3.

---

# PART 5 — FULL RESOURCE INDEX

### Tools
| | |
|---|---|
| Shadertoy | https://www.shadertoy.com |
| GLSL function reference | https://shaderific.com/glsl-functions · https://docs.gl |

### Video
| | |
|---|---|
| kishimisu — Intro to Shader Art Coding | https://www.youtube.com/watch?v=f4s1h2YETNY |
| — his Shadertoy | https://www.shadertoy.com/user/kishimisu |
| Freya Holmér — full playlist | https://www.youtube.com/playlist?list=PLImQaTpSAdsCnJon-Eir92SZMl7tPBS4Z |
| — Part 1: Basics, Blending & Textures | https://www.youtube.com/watch?v=kfM-yu0iQBk |
| — Part 2: Healthbars, SDFs & Lighting | https://www.youtube.com/watch?v=mL8U8tIiRRg |
| — Part 3: Normal Maps, Tangent Space & IBL | https://www.youtube.com/watch?v=E4PHFnvMzFc |
| — course doc, assignments + assets | https://docs.google.com/document/d/1h_7O9n5-q8Kb-rhnMJyF4Tab6i3lH97E4Ykn7tsP-zc/edit |
| Art of Code — Ray Marching for Dummies | https://www.youtube.com/watch?v=PGtv-dBi2wE |
| — Simple Shapes | https://www.youtube.com/watch?v=Ff0jJyyiVyw |
| — Basic Operators | https://www.youtube.com/watch?v=AfKGMUDWfuE |
| — Tips & Tricks | https://www.youtube.com/watch?v=Vmb7VGBVZJA |

### Written
| | |
|---|---|
| The Book of Shaders | https://thebookofshaders.com |
| — examples gallery | https://thebookofshaders.com/examples/ |
| Inigo Quilez — articles index | https://iquilezles.org/articles/ |
| — 3D SDF list | https://iquilezles.org/articles/distfunctions/ |
| — smooth minimum | https://iquilezles.org/articles/smin/ |
| — normals for an SDF | https://iquilezles.org/articles/normalsSDF/ |
| Maxime Heckel — Painting with Math | https://blog.maximeheckel.com/posts/painting-with-math-a-gentle-study-of-raymarching/ |
| GM Shaders (Xor) — SDFs | https://mini.gmshaders.com/p/sdf |
| Reinder Nijhoff — raymarching distance fields | https://reindernijhoff.net/2017/07/raymarching-distance-fields/ |

### Study target
| | |
|---|---|
| Seascape | https://www.shadertoy.com/view/Ms2SD1 |

Free material here genuinely is the best material, not a budget substitute. iq's site is what people in the industry actually reference; Book of Shaders is used in university courses; Freya's series was taught as an actual course at FutureGames and released publicly with assignments and assets.

---

# PART 6 — WHY THIS ORDER

**Shadertoy before any engine or framework:** zero boilerplate and instant visual feedback. No window creation, no buffer setup, no build step — you type an expression and see the result. That feedback loop is why shaders are the most enjoyable thing to learn in graphics, and it's destroyed by putting a rendering framework in front of it.

**kishimisu before Freya:** 20 minutes, Shadertoy-native, and it installs the per-pixel model in the exact environment you'll live in for four weeks. Freya's surface-shader framing lands better once you already know what a fragment shader is.

**2D before 3D:** per-pixel thinking is the actual skill. 3D adds a second unfamiliar thing on top.

**Noise before raymarching:** fBm is what makes raymarched scenes look like anything.

**All of it before Ben Cook:** a shadow map is a depth pass plus a comparison written in a fragment shader. Cascades, bias, PCF, PBR, deferred, post-processing — every one is shader code. Going at those without GLSL fluency means fighting two unfamiliar things at once.

Nothing gets dropped by this ordering. You lose four weeks of sequencing and gain a language you'll use for the rest of the track.