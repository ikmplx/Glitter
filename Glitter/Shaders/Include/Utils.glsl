vec3 DepthToPosition(float depth, vec2 screenUV)
{
	depth = depth * 2.0 - 1.0;
	screenUV = screenUV * 2.0 - 1.0;
	
	vec4 clipPos = vec4(screenUV, depth, 1.0);
	vec4 worldPos = invCombined * clipPos;
	
	return worldPos.xyz / worldPos.w; 
}
