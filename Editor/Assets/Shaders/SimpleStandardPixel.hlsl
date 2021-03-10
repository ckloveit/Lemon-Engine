struct PixelInput
{
	float4 Position : SV_POSITION;
	float4 Color 	: COLOR;
};

float4 MainPS(PixelInput Input) : SV_TARGET
{
	return Input.Color;
}