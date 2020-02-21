#include <iostream>
using namespace std;

#include "NoiseGenerator.h"

atomic<double> dFrequencyOutput = 0.0;

double MakeNoise(double dTime)
{

	double dOutput = 1.0 * (sin(dFrequencyOutput * 2 * 3.14159 * dTime) + sin((dFrequencyOutput + 20) * 2 * 3.14159 * dTime));

	return dOutput * 0.5;

	// square wave

	//if (dOutput > 0.0)
	//	return 0.2;
	//else
	//	return -0.2;

	// can change frequency with input, 440.0, 880.0, 220.2 (lower pitch)
	return 0.5 * sin(880 * 2 * 3.14159 * dTime); // picking frequency, 440 hertz is the A on the keyboard, but sign takes angular velocity, so need to convert
}

int main() {

	// hardware setup
	cout << "First step of Synthesizer" << endl;

	// aquires all sound hardware
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	// display findings
	for (auto d : devices) wcout << "Found Output Devices" << d << endl;

	// create sound machine
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	// Link noise function with sound machine
	sound.SetUserFunction(MakeNoise);


	// doubling in frequency, move up 1 octate
	double dOctaveBaseFrequency = 110.0; // A2
	double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

	while (1)
	{
		// Add a keyboard like piano

		bool bKeyPressed = false;
		for (int k = 0; k < 15; k++)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe"[k])) & 0x8000)
			{
				dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			dFrequencyOutput = 0.0;
		}
	}
	return 0;
}