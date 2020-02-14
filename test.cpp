#include "MidiFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace smf;

string PitchNameOctaveSensitive(int keynumber) {
	string pitches[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
        int octave = (keynumber / 12) - 1;
	int noteIndex = (keynumber % 12);
        string noteName = pitches[noteIndex];
	noteName = noteName + to_string(octave);
	return noteName;
}

string PitchNameClass(int keynumber) {
	string pitches[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
	int noteIndex = (keynumber % 12);
	string noteName = pitches[noteIndex];
	return noteName;
}

string VectorToString(vector<string> VectorIn) {
	string answer = "";
	for(int i=0; i<VectorIn.size(); i++) {
		if (VectorIn[i] != "R") answer = answer + VectorIn[i] + " ";
	}
	return answer;
}

vector<string> vecintersection(vector<string> vector1, vector<string> vector2) {
	vector<string> answer;
	for(int i=0; i<vector1.size(); i++) {
		for(int j=0; j<vector2.size(); j++) {
			if( (vector1[i] == vector2[j]) && (find(answer.begin(), answer.end(), vector2[j]) == answer.end()) ) {
				answer.push_back(vector2[j]);
			}
		}
	}
	return answer;
}

vector<string> vecdifference(vector<string> vector1, vector<string> vector2) {
	vector<string> answer;
	vector<string> vecsum = vecintersection(vector1, vector2);
	for(int i=0; i<vector1.size(); i++) {
		if(find(vecsum.begin(), vecsum.end(), vector1[i]) == vecsum.end()) {
			answer.push_back(vector1[i]);
		}
	}
	return answer;
}

void Display_Intersection(vector<vector<string>> vector1, vector<vector<string>> vector2, string title) {
	string FirstStr = VectorToString(vecintersection(vector1[0], vector2[0]));
	string SecondStr = VectorToString(vecintersection(vector1[1], vector2[1]));
	string ThirdStr = VectorToString(vecintersection(vector1[2], vector2[2]));

	cout << left << setw(30) << title << setw(50) << FirstStr << setw(40) << SecondStr << setw(30) << ThirdStr << endl;
	cout << endl;
}

void Display_Difference(vector<vector<string>> vector1, vector<vector<string>> vector2, string title) {
	string FirstStr = VectorToString(vecdifference(vector1[0], vector2[0]));
        string SecondStr = VectorToString(vecdifference(vector1[1], vector2[1]));
	string ThirdStr = VectorToString(vecdifference(vector1[2], vector2[2]));
		 
	cout << left << setw(30) << title << setw(50) << FirstStr << setw(40) << SecondStr << setw(30) << ThirdStr << endl;
	cout << endl;
}

void Great_Displayer(vector<vector<string>> vector1, vector<vector<string>> vector2, string vector1_title, string vector2_title) {
	string intersection = vector1_title + " " + vector2_title + " intersection";
	Display_Intersection(vector1, vector2, intersection);
	string difference1 = vector1_title + " - " + vector2_title;
	Display_Difference(vector1, vector2, difference1);
	string difference2 = vector2_title + " - " + vector1_title;
	Display_Difference(vector2, vector1, difference2);
	cout << endl;
}


int main() {
	MidiFile midifile;
	midifile.read("Ravel/Ravel__Jeux_deau.mid");
	if ( midifile.status() ) {
		cout << "Reading file successful" << endl;
	}

	int ticksper = midifile.getTicksPerQuarterNote();

	string OctAns;

	cout << "Should the comparison be octave sensitive? (Y or N):" << endl;
	cin >> OctAns;

	cout << "Number of tracks: " << midifile.getTrackCount() << endl << endl;

	float begA, endA, begB, endB;

	cout << "Enter beginning of fragment A in quarter notes:" << endl;

	cin >> begA;
	begA = begA * ticksper;

	cout << "Enter end of fragment A in quarter notes:" << endl;

	cin >> endA;
	endA = endA * ticksper;

	cout << "Enter beginning of fragment B in quarter notes:" << endl;

	cin >> begB;
	begB = begB * ticksper;

	cout << "Enter end of fragment B in quarter notes:" << endl;

	cin >> endB;
	endB = endB * ticksper;

	cout << endl;

	//string Asequence[60*16];
	vector<vector<string>> sequencesA;
	vector<string> initialA;
	initialA.assign(1, "R");
	sequencesA.assign(1, initialA);

	vector<vector<string>> sequencesB;
	vector<string> initialB;
	initialB.assign(1, "R");
	sequencesB.assign(1, initialB);

	vector<vector<string>> seqA_Track1;
	vector<vector<string>> seqB_Track1;
	vector<vector<string>> seqA_Track2;
	vector<vector<string>> seqB_Track2;

	vector<string> empty_vector = {};

	sequencesA.push_back(empty_vector);
	sequencesA.push_back(empty_vector);

	sequencesB.push_back(empty_vector);
	sequencesB.push_back(empty_vector);


	for(int i=0; i<3; i++) {
		//cout << "TrackNumber: " << i << endl;
		for(int j=0; j < midifile[i].size(); j++) {
			
			if (midifile[i][j].isNoteOn()) {
				int time = midifile[i][j].tick;
				int keynumber = midifile[i][j].getKeyNumber();
				string pitchname;
				if (OctAns == "Y") pitchname = PitchNameOctaveSensitive(keynumber);
				else pitchname = PitchNameClass(keynumber);

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
						if (NumberOfRepeats >= sequencesA.size()) { //sequencesA.resize(NumberOfRepeats + 1); sequencesA[NumberOfRepeats + 1] = {}; }
							sequencesA.push_back(empty_vector);
						}
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
						if (NumberOfRepeats >= sequencesB.size()) { //sequencesB.resize(NumberOfRepeats + 1); sequencesB[NumberOfRepeats + 1] = {}; }
							sequencesB.push_back(empty_vector);
						}
					}
				}
			
			}

		}

		/*
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
		*/
		
		if(i==1) {
			seqA_Track1 = sequencesA;
			seqB_Track1 = sequencesB;
		}
		
		if(i==2) {
			seqA_Track2 = sequencesA;
			seqB_Track2 = sequencesB;
		}

		initialA.assign(1, "R");
		sequencesA.assign(1, initialA);
		
		initialB.assign(1, "R");
		sequencesB.assign(1, initialB);
		
        	sequencesA.push_back(empty_vector);
	        sequencesA.push_back(empty_vector);

		sequencesB.push_back(empty_vector);
		sequencesB.push_back(empty_vector);


		cout << endl;
	}
	
	cout << left << setw(30) << "type" << setw(50) << "First appearence" << setw(40) << "Second appearence" << setw(30) << "Third appearence" << endl;
	cout << endl;
	cout << left << setw(30) << "A right" << setw(50) << VectorToString(seqA_Track1[0]) << setw(40) << VectorToString(seqA_Track1[1]) << setw(30) << VectorToString(seqA_Track1[2]) << endl;
	cout << endl;
	cout << left << setw(30) << "A left" << setw(50) << VectorToString(seqA_Track2[0]) << setw(40) << VectorToString(seqA_Track2[1]) << setw(30) << VectorToString(seqA_Track2[2]) << endl;
	cout << endl;
	cout << left << setw(30) << "B right" << setw(50) << VectorToString(seqB_Track1[0]) << setw(40) << VectorToString(seqB_Track1[1]) << setw(30) << VectorToString(seqB_Track1[2]) << endl;
	cout << endl;
	cout << left << setw(30) << "B left" << setw(50) << VectorToString(seqB_Track2[0]) << setw(40) << VectorToString(seqB_Track2[1]) << setw(30) << VectorToString(seqB_Track2[2]) << endl;
	cout << endl << endl;

	Great_Displayer(seqA_Track1, seqB_Track1, "AR", "BR");

	Great_Displayer(seqA_Track2, seqB_Track2, "AL", "BL");

	Great_Displayer(seqA_Track1, seqA_Track2, "AR", "AL");

	Great_Displayer(seqB_Track1, seqB_Track2, "BR", "BL");

	Great_Displayer(seqA_Track1, seqB_Track2, "AR", "BL");

	Great_Displayer(seqA_Track2, seqB_Track1, "AL", "BR");

	return 0;
}
