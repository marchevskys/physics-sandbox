// converts input value in radians to RGB rainbow
vec3 rainbow(float a)
{
    vec3 color = vec3(sin(a), sin(a + pi3), sin(a + pi3 * 2));
    return color * color;
}
