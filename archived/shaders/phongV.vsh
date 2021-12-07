#version 330 core
 
// input variables from vertex shder.
in vec3 v_position;
in vec3 v_normal;
 
// Automatic parameters.
uniform vec3 g_camera_position;
 
// Automatic parameters for lighting.
struct LightProperties {
	vec3 ambient;
	vec3 color;
	vec3 direction;
};
const int MAX_LIGHTS = 1;
uniform LightProperties g_lights[MAX_LIGHTS];
uniform int g_light_count;
 
// Material parameters for phong shading.
uniform vec3 m_diffuse = vec3(0.5, 0.5, 0.5);
uniform vec3 m_specular = vec3(0.0, 0.0, 0.0);
uniform float m_shininess = 0;
 
// output variable about fragment color.
out vec4 frag_color;
 
void main() {
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
 
	for (int i = 0; i < g_light_count; i++) {
		// Get the i-th light.
		LightProperties light = g_lights[i];
 
		vec3 light_direction = -normalize(light.direction);
		vec3 normal = normalize(v_normal);
 
		// Calculate the ambient component.
		ambient += m_diffuse * light.ambient;
		// Calculate the diffuse component.
		diffuse += m_diffuse * light.color * max(0, dot(normal, light_direction));
 
		vec3 view_direction = normalize(g_camera_position - v_position);
		vec3 half_vector = normalize(light_direction + view_direction);
 
		// Calculate the speculat component.
		specular += m_specular * light.color * pow(max(0, dot(normal, half_vector)), m_shininess);
	}
	
	vec3 color = min(ambient + diffuse + specular, vec3(1.0f));
	frag_color = vec4(color, 1.0);
	// frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}