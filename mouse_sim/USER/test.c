


int datatest()
{
	static int aim;
	aim++;
  return aim;
}

void testdatatra(int aim)
{
	static int buffer;
	buffer=aim;
}

