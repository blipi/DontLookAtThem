#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec4 vPosition;
IN vec4 vColor;

OUT vec4 color;

uniform mat4 MVP;

void main()
{
  gl_Position = MVP*vPosition;
  color = vColor;
}
