#include "MidiFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using namespace smf;

string PitchName(int keynumber) {
	string pitches[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
        int octave = (keynumber / 12) - 1;
	int noteIndex = (keynumber % 12);
        string noteName = pitches[noteIndex];
	noteName = noteName + to_string(octave);
	return noteName;
}


int main() {
	MidiFile midifile;
	midifile.read("Ravel/Ravel__Jeux_deau.mid");
	if ( midifile.status() ) {
		cout << "Reading file successful" << endl;
	}

	cout << "Ticks per quarter note: " <<  midifile.getTicksPerQuarterNote() << endl << endl;

	cout << "Number of tracks: " << midifile.getTrackCount() << endl << endl;

	int begA, endA, begB, endB;

	cout << "Enter beginning of fragment A in ticks:" << endl;

	cin >> begA;

	cout << "Enter end of fragment A in ticks:" << endl;

	cin >> endA;

	cout << "Enter beginning of fragment B in ticks:" << endl;

	cin >> begB;

	cout << "Enter end of fragment B in ticks:" << endl;

	cin >> endB;

	//string Asequence[60*16];
	vector<vector<string>> sequencesA;
	vector<string> initialA;
	initialA.assign(1, "R");
	sequencesA.assign(1, initialA);

	vector<vector<string>> sequencesB;
	vector<string> initialB;
	initialB.assign(1, "R");
	sequencesB.assign(1, initialB);

	for(int i=0; i<3; i++) {
		cout << "TrackNumber: " << i << endl;
		for(int j=0; j < midifile[i].size(); j++) {
			
			if (midifile[i][j].isNoteOn()) {
				int time = midifile[i][j].tick;
				int keynumber = midifile[i][j].getKeyNumber();
				string pitchname = PitchName(keynumber);

				if (time >= begA && time < endA) {
					int IsAdded = 0;
					int NumberOfRepeats = 0;
					while (IsAdded == 0) {
						if ( find( sequencesA[NumberOfRepeats].begin(), sequencesA[NumberOfRepeats].end(), pitchname) != sequencesA[NumberOfRepeats].end()) {
							NumberOfRepeats++;
						}
						else {
							sequencesA[NumberOfRepeats].push_back(pitchname);
							IsAdded = 1;
						}
						if (NumberOfRepeats >= sequencesA.size()) sequencesA.resize(NumberOfRepeats + 1);
					}
				}

				
				if (time >= begB && time < endB) {
					int IsAdded = 0;
					int NumberOfRepeats = 0;
					while (IsAdded == 0) {
						if ( find( sequencesB[NumberOfRepeats].begin(), sequencesB[NumberOfRepeats].end(), pitchname) != sequencesB[NumberOfRepeats].end()) {
							NumberOfRepeats++;
						}
						else {
							sequencesB[NumberOfRepeats].push_back(pitchname);
							IsAdded = 1;
						}
						if (NumberOfRepeats >= sequencesB.size()) sequencesB.resize(NumberOfRepeats + 1);
					}
				}
			
			}

		}

		cout << "For fragment A:" << endl;

		for (int i=0; i < sequencesA.size(); i++) {
			for (int j=0; j < sequencesA[i].size(); j++) {
				if (sequencesA[i][j] != "R") cout << sequencesA[i][j] << " ";
			}
			cout << endl;
		}
		
		cout << "For fragment B:" << endl;

		for (int i=0; i < sequencesB.size(); i++) {
			for (int j=0; j < sequencesB[i].size(); j++) {
				if (sequencesB[i][j] != "R") cout << sequencesB[i][j] << " ";
			}
			cout << endl;
		}

		initialA.assign(1, "R");
		sequencesA.assign(1, initialA);
		
		initialB.assign(1, "R");
		sequencesB.assign(1, initialB);

		cout << endl;
	}
	return 0;
}
