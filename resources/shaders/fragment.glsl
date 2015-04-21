#if __VERSION__<130
    #define IN varying
    #define OUT varying
#else
    #define IN in
    #define OUT out
#endif

uniform sampler2D texSampler;

IN vec2 texCoords;
IN vec4 color;

void main()
{
   //gl_FragColor = color;
   gl_FragColor = texture2D(texSampler, texCoords.st);
}
