#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    const char* highscore = "highscores";
    int topscores[5] = {0,0,0,0,0};
    int shifter[5] = {0,0,0,0,0};
    
    ifstream fileIN(highscore,ios::binary); //open file for reading as binary, take the top 5 scores
        for (int i = 0; i < 5; i++) {
            if (!fileIN.eof()) {
                fileIN.read((char*)&topscores[i], sizeof(topscores[i])); //get 5 high scores
            }
        }

        fileIN.seekg(0); //reset pos

        for (int i = 0; i < 5; i++) {
            if (!fileIN.eof()) {
                fileIN.read((char*)&shifter[i], sizeof(shifter[i])); //get 5 high scores
            }
        }

    fileIN.close();

    //display top scores
    cout << "highscores\n";
    for (int i = 0; i < 5; i++) {
        cout << topscores[i] << "\n";
    } 
    
    //take score
    int score;
    bool topscorewritten = false;
    cout << "enter your score: ";
    cin >> score;



    //ask if score is greater than any of the current scores
    for (int i = 0; i < 5; i++) {
        if (score > topscores[i]) {
            //write the score
            topscores[i] = score;

            //shift the rest down
            for (int j = i; j < 4; j++) {
                topscores[j + 1] = shifter[j];
            }

            //exit the loop
            i = 5;


        }
            
    }

    ofstream fileOUT(highscore,ios::binary); //open for writing as binary
        fileOUT.seekp(0); //set seek pos to beginning of file
        for (int i = 0, byte = 0; i < 5; i++, byte += 4) {
            fileOUT.seekp(byte); //advance to the current byte
            //write the array
            fileOUT.write((char*)&topscores[i], sizeof(topscores));            
        }


    fileOUT.close();
}   
