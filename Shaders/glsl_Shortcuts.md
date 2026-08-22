1.2 — GLSL reference card

Types float · vec2 vec3 vec4 · mat2 mat3 mat4 · int · bool No classes, no pointers, no new, no recursion, no dynamic allocation. float is the default currency.

Swizzling — v.xy · v.rgb · v.xxx · v.wz. Reorder or duplicate components inline. col.bgr flips red and blue. Used constantly.

Component-wise math — vec3 * vec3 multiplies element by element, not a dot product.

Constructors — vec3(1.0) fills all three. vec4(rgb, 1.0) composes from a vec3 plus a scalar.

Function	What it does	Where you'll use it
mix(a, b, t)	linear blend	every colour blend, every fade
clamp(x, lo, hi)	bound a value	keeping colours in 0..1
step(edge, x)	0 below, 1 above — hard	binary masks
smoothstep(a, b, x)	0→1 with a soft ramp	every soft edge you ever write
fract(x)	fractional part	tiling, repetition, hashes
floor(x)	round down	cell index in a grid
mod(x, y)	remainder	domain repetition
length(v)	magnitude	distance to a point → circles
distance(a, b)	length(a - b)	same
normalize(v)	unit vector	ray directions, normals
dot(a, b)	projection	diffuse lighting, angles
cross(a, b)	perpendicular	camera bases
atan(y, x)	angle	polar coordinates
abs sign min max pow exp sin cos	scalar utility	everywhere

These are hardware-fast. Reach for them instead of writing if.

Branching is expensive. Neighbouring pixels taking different if paths means the GPU runs both and throws one away. Prefer step / smoothstep / mix to express a choice as arithmetic — same reasoning as SIMD lanes.

Floats need decimals. 1 is an int; 1.0 is a float. float x = 1; is a compile error. Bites on day one, then never again.