/*
    Homework assignment #1 of Data Structure 2026, NTPU CSIE.
    Goals: implementaton of text/image run-length coding.
*/

#include "RunLengthCoding.h"
#include "ImageUtil.h"

// Global variables.
const float waitTime = 1.0f;    // For delaying the next test.

// Test text inputs.
const int nTestTexts = 5;
const char* testTexts[nTestTexts] = {
    "wwwwaaadexxxxxx",
    "aaaaaaaaaaaaaaaaaaabbbbbbbbccccccccccccccccccccccccccccccccccddd",
    ">>>>>>>>>>>>OOOOOO<<<<<<<<<<<<<<<<",
    "!!!!!!!!!!!^^^^^^^^<<<<<<<<WWWWWWWWWWWWWWWWW",
    "pneumonoultramicroscopicsilicovolcanoconiosis"
};

// Test image inputs.
const int nTestImages = 5;
const char* testImageFiles[nTestImages] = {
    "TestInputs/Simple.png",
    "TestInputs/SatoruGojo.png",
    "TestInputs/AttackOnTitan.png",
    "TestInputs/SpyFamily.png",
    "TestInputs/DemonSlayer.png"
};

// Method prototypes.
bool ValidateTextCoding(const Text&, const Text&);
void RunTextCodingCheck();
void RunImageCodingCheck();
void Sleep(float seconds);



int main(int argc, char** argv)
{
    // Part1: text encoding/decoding.
    RunTextCodingCheck();

    cout << endl;
    cout << "=================================================================" << endl;
    cout << endl;

    // Part2: image encoding/decoding.
    RunImageCodingCheck();


    return 0;
}

void RunTextCodingCheck()
{
    for (int i = 0; i < nTestTexts; ++i) {
        cout << "Check text input " << i << endl;
        cout << "[INPUT] original test text input: " << testTexts[i] << endl;

        // Prepare for the test text.
        const char* testText = testTexts[i];      
        Text origText(testText);
        
        // Perform encoding.
        TextEncodeData textEncodeData;
        RunLengthCoding::EncodeText(origText, textEncodeData);
        
        // Perform decoding.
        Text decodeText;
        RunLengthCoding::DecodeText(textEncodeData, decodeText);
        
        // Validation of text encoding/decoding.
        if (ValidateTextCoding(origText, decodeText))
            cout << "[RESULT] test text input " << i << " passed!" << endl;
        else
            cout << "[RESULT] test text input " << i << " failed..." << endl;
        
        cout << "-------------------------------------------------------------" << endl;
        Sleep(waitTime);
    }
}

void RunImageCodingCheck()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    for (int i = 0; i < nTestImages; ++i) {
        cout << "Check image input " << i << ": " << endl;
        
        // Load the test image.
        cv::Mat origImg = cv::imread(testImageFiles[i], cv::IMREAD_GRAYSCALE);
        // ImageUtil::ShowImage(origImg, "Image before comprssion (downsampled)");
        
        // Perform encoding.
        ImageEncodeData imgEncodeData;
        RunLengthCoding::EncodeImage(origImg, imgEncodeData);
        
        // Perform decoding.
        cv::Mat decodeImg;
        RunLengthCoding::DecodeImage(imgEncodeData, decodeImg);
        // ImageUtil::ShowImage(decodeImg, "Image after decomprssion (downsampled)");
        
        // Validation of image encoding/decoding.
        if (ImageUtil::SameUCharImage(origImg, decodeImg))
            cout << "[RESULT] test image input " << i << " passed!" << endl;
        else
            cout << "[RESULT] test image input " << i << " failed..." << endl;
        
        cout << "-------------------------------------------------------------" << endl;
        Sleep(waitTime);
    }
}

bool ValidateTextCoding(const Text& origText, const Text& decodeText)
{
    if (origText.length != decodeText.length)
        return false;

    for (int i = 0 ; i < origText.length ; ++i)
        if (origText.data[i] != decodeText.data[i])
            return false;
    return true;
}

void Sleep(float seconds)
{
    clock_t startClock = clock();
    float secondsAhead = seconds * CLOCKS_PER_SEC;
    // Do nothing until the elapsed time has passed.
    while(clock() < startClock+secondsAhead);
}


