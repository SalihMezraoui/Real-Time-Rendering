#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 27) out;

in Vertex{
	vec3 vposition;
	float vlength;
	vec3 vnormal;
}vertex[];

out vec3 out_position;
out float out_length;
out vec3 out_normal;

float scaleTriangle = 0.58f;
float scaleLength = 0.67f;

void emitTriangle(vec3 v0, vec3 v1, vec3 v2, float length);
void emitTriangleWithNormal( vec3 v0, vec3 v1, vec3 v2, float length, vec3 normal);

void main() {

    vec3 p[3];
    p[0] = vertex[0].vposition;
    p[1] = vertex[1].vposition;
    p[2] = vertex[2].vposition;
    float len = vertex[0].vlength;

    if( len <= 0.f ) {
        emitTriangleWithNormal( p[0], p[1], p[2], 0.0f, vertex[0].vnormal);
    }
    else {
        vec3 q[3];
        vec3 t;
        float nextLength;

        //berechne den normal vector und die Länge
        vec3 a = p[1] - p[0];
        vec3 b = p[2] - p[0];
        vec3 normal = cross(a, b);
        normal = normalize(normal);
        vec3 height = normal * len;
        vec3 center = (p[0] + p[1] + p[2]) / 3;


        for(int i = 0; i < 3; i++) {
            vec3 tmp = (p[i] - center) * scaleTriangle;
            q[i] = center + height + tmp;
        }

        float topMantle = length(a)*0.2f;
        t = center + height + normal * topMantle;
        nextLength = len * scaleLength;

        //Generate the mantle
        for( int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            emitTriangle( p[i], q[j], q[i], 0.0f);
            emitTriangle( p[i], p[j], q[j], 0.0f);
        }

        //genrate top
        for( int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            emitTriangle( q[i], q[j], t, nextLength);
        }
    }
}

void emitTriangleWithNormal( vec3 v0, vec3 v1, vec3 v2, float len, vec3 normal) {
    out_length = len;
    out_normal = normal;
    out_position = v0;
    EmitVertex();
    out_position = v1;
    EmitVertex();
    out_position = v2;
    EmitVertex();
    EndPrimitive();
}

void emitTriangle( vec3 v0, vec3 v1, vec3 v2, float length) {

    vec3 normal = normalize(cross(v1-v0, v2-v0));
    emitTriangleWithNormal(v0, v1, v2, length, normal);
}