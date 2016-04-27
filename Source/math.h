
void normalize(CVector angles) {

	while (angles.x > 180.0f)
		angles.x -= 360.0f;

	while (angles.x < -180.0f)
		angles.x += 360.0f;

	while (angles.y > 180.0f)
		angles.y -= 360.0f;

	while (angles.y < -180.0f)
		angles.y += 360.0f;

	while (angles.z > 180.0f)
		angles.z -= 360.0f;

	while (angles.z < -180.0f)
		angles.z += 360.0f;

}

CVector ClampAngles(CVector angles) {

	while (angles.y > 180)
		angles.y -= 360;

	while (angles.y < -180)
		angles.y += 360;

	if (angles.x > 89.0f)
		angles.x = 89.0f;

	if (angles.x < -89.0f)
		angles.x = -89.0f;

	angles.z = 0;
	return angles;

}