#include "RunLengthCoding.h"

// helper
static int digitCount(int number)
{
    if (number == 0)
        return 1;

    int nDigits = 0;
    while (number > 0)
    {
        number /= 10;
        nDigits++;
    }
    return nDigits;
}

static int readNumber(const char *src, int &readPos, int len)
{
    int count = 0;
    while (readPos < len && (src[readPos] >= '0' && src[readPos] <= '9'))
    {
        count = count * 10 + src[readPos] - '0';
        readPos++;
    }
    return count;
}

void RunLengthCoding::EncodeText(const Text &origText, TextEncodeData &encodeData)
{
    // Check if the input is valid.
    if (origText.data == nullptr || origText.length <= 0)
    {
        cerr << "Invalid text input for encoding..." << endl;
        exit(1);
    }
    cout << "Original length: " << origText.length << endl;

    // Implement the run-length text encoding algorithm here.
    // -------------------------------------------------------------------------------------------
    // TODO: add code here to encode the origText and put your result in encodeData.
    // Note: you should fill both encodeData.data and encodeData.dataLength.
    // -------------------------------------------------------------------------------------------

    int encodedLength = 0, i = 0;

    while (i < origText.length)
    {
        int j = i;
        while (j < origText.length && origText.data[i] == origText.data[j])
            j++;
        encodedLength += 1 + digitCount(j - i);
        i = j;
    }

    encodeData.data = new char[encodedLength + 1];
    encodeData.dataLength = encodedLength;

    int writePos = 0;
    i = 0;
    while (i < origText.length)
    {
        int j = i;
        while (j < origText.length && origText.data[i] == origText.data[j])
            j++;
        int runLength = j - i, nDigits = digitCount(runLength);
        if (writePos + 1 + nDigits > encodedLength)
            break;

        encodeData.data[writePos] = origText.data[i];
        writePos++;

        for (int k = nDigits; k > 0; k--)
        {
            encodeData.data[writePos + k - 1] = (char)('0' + runLength % 10);
            runLength /= 10;
        }
        writePos += nDigits;
        i = j;
    }
    encodeData.data[writePos] = '\0';

    // Print out encode information (PLEASE DO NOT TOUCH THIS CODE).
    if (encodeData.data != nullptr)
    {
        cout << "Encode string: " << encodeData.data << endl;
        cout << "Encode length: " << encodeData.dataLength << endl;
        cout << "Compression ratio: " << (float)origText.length / (float)encodeData.dataLength << endl;
    }
}

void RunLengthCoding::DecodeText(const TextEncodeData &encodeData, Text &decodeText)
{
    // Implement the run-length text decoding algorithm here.
    // -------------------------------------------------------------------------------------------
    // TODO: add your code here to decode the data from encodeData and put the result into decodeText.
    // Note: you should fill both decodeText.data and decodeText.length.
    // -------------------------------------------------------------------------------------------
    int decodeLength = 0, i = 0;

    while (i < encodeData.dataLength)
    {
        i++;
        decodeLength += readNumber(encodeData.data, i, encodeData.dataLength);
    }

    decodeText.data = new char[decodeLength + 1];
    decodeText.length = decodeLength;

    int writePos = 0;
    i = 0;

    while (i < encodeData.dataLength)
    {
        char ch = encodeData.data[i];
        i++;
        int runLength = readNumber(encodeData.data, i, encodeData.dataLength);

        for (int k = 0; k < runLength && writePos < decodeLength; k++)
        {
            decodeText.data[writePos] = ch;
            writePos++;
        }
    }
    decodeText.data[writePos] = '\0';
    // Print out decode result (PLEASE DO NOT TOUCH THIS CODE).
    if (decodeText.data != nullptr)
    {
        cout << "Decode string: " << decodeText.data << endl;
    }
}

void RunLengthCoding::EncodeImage(const cv::Mat &image, ImageEncodeData &encodeData)
{
    // Keep image information (PLEASE DO NOT TOUCH THIS CODE).
    encodeData.imageWidth = image.size().width;
    encodeData.imageHeight = image.size().height;
    encodeData.imageType = image.type();

    // Obtain raw pixel data (PLEASE DO NOT TOUCH THIS CODE).
    uchar *rawImageData = image.data;
    int nPixels = encodeData.imageWidth * encodeData.imageHeight;
    int rawSize = sizeof(uchar) * nPixels;
    cout << "Raw image size: " << rawSize << " bytes" << endl;

    // Implement the run-length image encoding algorithm here.
    // -------------------------------------------------------------------------------------------
    // TODO: add your code here to encode the image pixel data and put the result into encodeData.
    // Note: you should fill both encodeData.data and encodeData.numData.
    // -------------------------------------------------------------------------------------------
    int numRuns = 0, i = 0;
    while (i < nPixels)
    {
        int j = i;
        while (j < nPixels && rawImageData[i] == rawImageData[j])
            j++;
        numRuns++;
        i = j;
    }
    encodeData.data = new ImageRunLengthData[numRuns];
    encodeData.numData = numRuns;

    i = 0;
    int writePos = 0;
    while (i < nPixels && writePos < numRuns)
    {
        int j = i;
        while (j < nPixels && (rawImageData[i] == rawImageData[j]))
            j++;
        encodeData.data[writePos] = ImageRunLengthData(rawImageData[i], j - i);
        writePos++;
        i = j;
    }
    if (writePos != encodeData.numData)
        cerr << "BUG: run count mismatch" << endl;

    // Print out encode information (PLEASE DO NOT TOUCH THIS CODE).
    int encodedSize = sizeof(ImageRunLengthData) * encodeData.numData;
    cout << "Encoded size: " << encodedSize << " bytes" << endl;
    cout << "Compression ratio: " << (float)rawSize / (float)encodedSize << endl;
}

void RunLengthCoding::DecodeImage(const ImageEncodeData &encodeData, cv::Mat &decodeImage)
{
    // Implement the run-length image decoding algorithm here.

    int nPixels = encodeData.imageWidth * encodeData.imageHeight;
    uchar *decodeData = new uchar[nPixels];
    fill(decodeData, decodeData + nPixels, (uchar)0);
    // -------------------------------------------------------------------------------------------
    // TODO: add your code here to decode the data from encodeData and fill into the decodeData array.
    // -------------------------------------------------------------------------------------------
    int writePos = 0;
    for (int k = 0; k < encodeData.numData; k++)
    {
        ImageRunLengthData run = encodeData.data[k];
        for (int i = 0; i < run.count && writePos < nPixels; i++)
        {
            decodeData[writePos] = run.value;
            writePos++;
        }
    }

    // Fill the data in decodeData into OpenCV Mat (PLEASE DO NOT TOUCH THIS CODE).
    decodeImage = cv::Mat(cv::Size(encodeData.imageWidth, encodeData.imageHeight), encodeData.imageType);
    for (int h = 0; h < decodeImage.size().height; ++h)
    {
        for (int w = 0; w < decodeImage.size().width; ++w)
        {
            decodeImage.at<uchar>(h, w) = decodeData[h * decodeImage.size().width + w];
        }
    }
}
