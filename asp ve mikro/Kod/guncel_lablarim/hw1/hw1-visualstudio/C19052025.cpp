#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

#define pixel_max(a) ((a) <= 255 ? (a) : 255)
#define pixel_min(a) ((a) >= 0 ? (a) : 0)

// Function to read an image in grayscale
unsigned char* readImage(const char* filename, int& width, int& height, int& channels) {
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 1); // Load as grayscale
    if (!image) {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }
    std::cout << "Image loaded successfully!" << std::endl;
    std::cout << "Width: " << width << ", Height: " << height << ", Channels: " << channels << std::endl;
    return image;
}

// Function to write an image to a PNG file
bool writeImage(const char* filename, unsigned char* image, int width, int height) {
    if (!image) {
        std::cerr << "Image data is null before writing!" << std::endl;
        return false;
    }
    if (width <= 0 || height <= 0) {
        std::cerr << "Invalid image dimensions: width = " << width << ", height = " << height << std::endl;
        return false;
    }
    // For grayscale images, stride is the same as the width
    int stride = width;
    if (stbi_write_png(filename, width, height, 1, image, stride) == 0) {
        std::cerr << "Failed to write the image to file: " << filename << std::endl;
        return false;
    }
    std::cout << "Image written successfully to: " << filename << std::endl;
    return true;
}


int main() {
    // Input and output file paths
    const char* inputFilename = "images/img5.png";
    const char* outputFilename1 = "inverted.png";
    const char* outputFilename2 = "histogram.png";

    // Image data variables
    int width, height, channels; // channels = 1 (grayscale)
    unsigned int number_of_pixels;

    // Read the input image
    unsigned char* image = readImage(inputFilename, width, height, channels);
    if (!image) 
        return -1; // Exit if the image failed to load

    // Allocate memory for the output image
    unsigned char* outputImage = new unsigned char[width * height];
    if (!outputImage) {
        std::cerr << "Failed to allocate memory for output image!" << std::endl;
        stbi_image_free(image);
        return -1;
    }

    // image is 1d array 
    // with length = width * height
    // pixels can be used as image[i] 
    // pixels can be updated as image[i] = 100, etc.
    // a pixel is defined as unsigned char
    // so a pixel can be 255 at max, and 0 at min.

    /*  QUESTION-1  */
    
    /* Q-1 Inverse the colors of image. 
    Inverse -> pixel_color = 255 - pixel_color */

    number_of_pixels = width * height;
    __asm {
        MOV ECX, 0
    Q1_START:
        CMP ECX, number_of_pixels
        JAE Q1_END                // i < number_of_pixels
        // ------- loop başlangıcı ------- //
        MOV EBX, image            // EBX = image
        ADD EBX, ECX              // EBX = &image[i]

        MOV AH, 0
        MOV AL, BYTE PTR [EBX]    // AL = image[i]

        MOV DH, 0
        MOV DL, 255               // DL = 255
        SUB DL, AL                // DL = 255 - image[i]

        MOV EBX, outputImage      // EBX = out
        ADD EBX, ECX              // EBX = &out[i]
        MOV BYTE PTR [EBX], DL    // out[i] <- inverted
        // ------- loop bitişi ------- //
        INC ECX
        JMP Q1_START
    Q1_END:
    }


    // Write the modified image as output_image1.png
    if (!writeImage(outputFilename1, outputImage, width, height)) {
        stbi_image_free(image);
        return -1;
    }
    stbi_image_free(outputImage); // Clear the outputImage.

    /*  QUESTION-2  */
    /* Histogram Equalization */

    outputImage = new unsigned char[width * height];
    if (!outputImage) {
        std::cerr << "Failed to allocate memory for output image!" << std::endl;
        stbi_image_free(image);
        return -1;
    }

    unsigned int* hist = (unsigned int*)malloc(sizeof(unsigned int) * 256);
    unsigned int* cdf = (unsigned int*)malloc(sizeof(unsigned int) * 256);

    // Check if memory allocation succeeded
    if (hist == NULL) {
        std::cerr << "Memory allocation for hist failed!" << std::endl;
        return -1;
    }
    if (cdf == NULL) {
        std::cerr << "Memory allocation for cdf failed!" << std::endl;
        free(hist);
        return -1;
    }

    // Both hist and cdf are initialized as zeros.
    for (int i = 0; i < 256; i++) {
        hist[i] = 0;
        cdf[i] = 0;
    }

    // You can define new variables here... As a hint some variables are already defined.
    unsigned int min_cdf, range;
    number_of_pixels = width * height;


    // Q-2 (a) - Compute the histogram of the input image.
    __asm {
        MOV ECX, 0
    Q2A_START:
        CMP ECX, number_of_pixels
        JAE Q2A_END
        // ------- loop başlangıcı ------- //
        MOV ESI, image            // ESI = image
        ADD ESI, ECX              // ESI = &image[i]
        XOR EAX, EAX
        MOV AL, BYTE PTR [ESI]    // EAX = image[i]
        SHL EAX, 2                // EAX = 4*image[i]

        MOV EDI, hist
        ADD EDI, EAX              // EBX = hist+4*image[i] = &hist[image[i]]
        MOV EDX, [EDI]            // EDX = hist[image[i]]
        INC EDX                   // EDX = hist[image[i]] + 1
        MOV [EDI], EDX            // hist[image[i]] = hist[image[i]] + 1
        // ------- loop bitişi ------- //
        INC ECX
        JMP Q2A_START
    Q2A_END:
    }


    /* Q-2 (b) - Compute the Cumulative Distribution Function cdf
                    and save it to cdf array which is defined above. */

    // CDF Calculation (cdf[i] = cdf[i-1] + hist[i])
    __asm {
        MOV EBX, hist
        MOV EAX, [EBX]  // EAX = hist[0]
        MOV EBX, cdf
        MOV [EBX], EAX  // cdf[0] = hist[0]

        MOV ECX, 1
    Q2B_START:
        CMP ECX, 256
        JAE Q2B_END
        // ------- loop başlangıcı ------- //
        // hist[i] erişimi
        MOV EDX, ECX       // EDX = i
        SHL EDX, 2         // EDX = 4*i
        MOV EBX, hist      // EBX = &hist[0]
        ADD EBX, EDX       // EBX = &hist[i]
        MOV EAX, [EBX]     // EAX = hist[i]

        // cdf[i-1] erişimi
        MOV EDX, ECX       // EDX = i
        DEC EDX            // EDX = i-1
        SHL EDX, 2         // EDX = 4*(i-1)
        MOV EBX, cdf       // EBX = &cdf[0]
        ADD EBX, EDX       // EBX = &cdf[i-1]
        ADD EAX, [EBX]     // EAX = hist[i] + cdf[i-1] = cdf[i]

        // toplamın atanması
        MOV EDX, ECX       // EDX = i
        SHL EDX, 2         // EDX = 4*i
        MOV EBX, cdf       // EBX = &cdf[0]
        ADD EBX, EDX       // EBX = &cdf[i]
        MOV [EBX], EAX     // cdf[i] değerini yaz

        // ------- loop bitişi ------- //
        INC ECX
        JMP Q2B_START
    Q2B_END:
    }


    /* Q-2 (c) - Normalize the Cumulative Distribution Funtion 
                    that you have just calculated on the same cdf array. */

    // Normalized cdf[i] = ((cdf[i] - min_cdf) * 255) / range
    // range = (number_of_pixels - min_cdf)

    __asm {
        // --------------------- MIN_CDF HESAPLAMA --------------------- //
        MOV EDX, 0xFFFFFFFF   // min değeri tutması için
        MOV ECX, 0
    MINCDF_START:
        CMP ECX, 256
        JAE MINCDF_END
        // loop başlangıcı
        MOV EAX, ECX          // EAX = i
        SHL EAX, 2            // EAX = 4*i
        MOV EBX, cdf          // EBX = &cdf[0]
        ADD EBX, EAX          // EBX = &cdf[i]
        MOV EBX, [EBX]        // EBX = cdf[i]
        
        CMP EBX, EDX
        JAE NOT_NEW_MIN       // cdf[i] >= min ise bulunmadı
        CMP EBX, 0
        JE NOT_NEW_MIN        // cdf[i] = 0 ise geçersiz, bulunmadı
        MOV EDX, EBX
    NOT_NEW_MIN:
        // loop bitişi
        INC ECX
        JMP MINCDF_START
    MINCDF_END:
        MOV min_cdf, EDX

        MOV EAX, number_of_pixels
        SUB EAX, EDX
        MOV range, EAX
        
        // --------------------- NORMALIZE ETME --------------------- //
        // cdf[i] = ((cdf[i] - min_cdf) * 255) / range

        MOV ECX, 0
    NORM_START:
        CMP ECX, 256
        JAE NORM_END
        // loop başlangıcı
        MOV EAX, ECX          // EAX = i
        SHL EAX, 2            // EAX = 4*i
        MOV EBX, cdf          // EBX = &cdf[0]
        ADD EBX, EAX          // EBX = &cdf[i]
        MOV EAX, [EBX]        // EAX = cdf[i]
        SUB EAX, min_cdf      // EAX = cdf[i] - min_cdf
        XOR EDX, EDX          // EDX:EAX = cdf[i] - min_cdf
        MOV EBX, 255
        MUL EBX               // EDX:EAX = (cdf[i] - min_cdf) * 255
        MOV EBX, range
        DIV EBX               // EDX:EAX = ((cdf[i] - min_cdf) * 255) / range

        MOV ESI, cdf          // ESI = &cdf[0]
        MOV EBX, ECX          // EBX = i
        SHL EBX, 2            // EBX = 4*i
        ADD ESI, EBX          // ESI = &cdf[i]
        MOV [ESI], EAX        // cdf[i] = normalize değer
        // loop bitişi
        INC ECX
        JMP NORM_START
    NORM_END:
    }


    /* Q-2 (d) - Apply the histogram equalization on the image.
                    Write the new pixels to outputImage. */
	// Here you only need to get a pixel from image, say the value of pixel is 107
	// Then you need to find the corresponding cdf value for that pixel
	// The output for the pixel 107 will be cdf[107]
	// Do this for all the pixels in input image and write on output image.
    __asm {
        MOV ECX, 0
    Q2D_START:
        CMP ECX, number_of_pixels
        JAE Q2D_END
        // loop başlangıcı
        MOV EBX, image            // EBX = image
        ADD EBX, ECX              // EBX = &image[i]
        XOR EAX, EAX
        MOV AL, BYTE PTR [EBX]    // EAX = image[i]
        SHL EAX, 2                // EAX = 4*image[i]

        MOV ESI, cdf              // ESI = &cdf[0]
        ADD ESI, EAX              // ESI = &cdf[image[i]]
        MOV EAX, [ESI]            // EAX = cdf[image[i]]

        MOV EBX, outputImage      // EBX = &outputImage
        ADD EBX, ECX              // EBX = &outputImage[i]
        MOV BYTE PTR [EBX], AL    // outputImage[i] = cdf[image[i]]

        // loop bitişi
        INC ECX
        JMP Q2D_START
    Q2D_END:
    }


    // Write the modified image
    if (!writeImage(outputFilename2, outputImage, width, height)) {
        stbi_image_free(image); 
        return -1;
    }

    // Free the image memory
    stbi_image_free(image);
    stbi_image_free(outputImage);

    return 0;
}
