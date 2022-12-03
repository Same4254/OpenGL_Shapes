#include "Utils/Utils.h"

#include <sys/types.h>
#include <inttypes.h>
#include <netinet/in.h>

#if __BIG_ENDIAN__
    #define htonll(x)   (x)
    #define ntohll(x)   (x)
#else
    #define htonll(x)   ((((uint64_t)htonl(x&0xFFFFFFFF)) << 32) + htonl(x >> 32))
    #define ntohll(x)   ((((uint64_t)ntohl(x&0xFFFFFFFF)) << 32) + ntohl(x >> 32))
#endif

uint64_t Utils_FileIO_ReadFile(const char *filePath, char **buffer) {
	FILE *fp = fopen(filePath, "rb");

    uint64_t file_byte_length = 0;

	if (fp) {
		fseek(fp, 0, SEEK_END);
		file_byte_length = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		*buffer = malloc(file_byte_length);
		fread(*buffer, 1, file_byte_length, fp);

        fclose(fp);
	}

	return file_byte_length;
}

uint64_t Utils_FileIO_ReadFileTerminated(const char *filePath, char **buffer_ptr) {
	FILE *fp = fopen(filePath, "rb");

    uint64_t file_byte_length = 0;

	if (fp) {
		fseek(fp, 0, SEEK_END);
		file_byte_length = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		*buffer_ptr = malloc(file_byte_length + 1);
		fread(*buffer_ptr, 1, file_byte_length, fp);
		(*buffer_ptr)[file_byte_length] = '\0';
        
        fclose(fp);
	}

    return file_byte_length;
}

uint32_t Utils_FileIO_CRC32(const char *data) {

}

typedef struct {
	uint32_t width;
	uint32_t height;
	uint8_t bitDepth;
	uint8_t colorType;
	uint8_t compressionType;
	uint8_t filterMethod;
	uint8_t interlaceMethod;
} IHDR_Chunk;

float* Utils_FileIO_ReadPNG_RGB(const char *filePath) {
    float* rgb_values = NULL;

	char *original_file_content = NULL;
    uint64_t length = Utils_FileIO_ReadFile(filePath, &original_file_content);

    char *shifted_content = original_file_content;

    if (original_file_content == NULL) {
        printf("ERROR: File [%s] could not be found!\n", filePath);
    }

    const uint64_t correct_header = 0x89504E470D0A1A0A;
    const uint64_t read_header = ntohll(*(uint64_t*) shifted_content);

    // header check
    if (read_header != correct_header) {
        printf("PNG (%s) had incorrect header. Was: %lx. Expected: %lx\n", filePath, read_header, correct_header);
        free(shifted_content);
        return NULL;
    }

    shifted_content += 8;

    printf("-------------------\n");
    printf("PNG Image Loading (%s)\n", filePath);
    printf("Length: %lu\n", length);
    printf("Chunks:\n");

    IHDR_Chunk ihdr_chunk;

    // Chunks
    while (shifted_content != original_file_content + length) {
        uint32_t chunk_length = ntohl(*((uint32_t*) shifted_content));
        shifted_content += 4;

        uint32_t chunk_type = ntohl(*((uint32_t*) shifted_content));
        shifted_content += 4;

        char *chunk_data = shifted_content;
        shifted_content += chunk_length;

        uint32_t crc = ntohl(*(uint32_t*) shifted_content);
        shifted_content += 4;

        printf("\tChunk Type: 0x%x, Length: %u, CRC-32: 0x%x\n", chunk_type, chunk_length, crc);

        switch (chunk_type) {
            // IHDR
            case 0x49484452:
                ihdr_chunk.width = ntohl(*(uint32_t*)chunk_data);
                ihdr_chunk.height = ntohl(*(uint32_t*)(chunk_data + 4));
                ihdr_chunk.bitDepth = chunk_data[8];
                ihdr_chunk.colorType = chunk_data[9];
                ihdr_chunk.compressionType = chunk_data[10];
                ihdr_chunk.filterMethod = chunk_data[11];
                ihdr_chunk.interlaceMethod = chunk_data[12];

                printf("\tIHDR Chunk. Width: %u, Height: %u, Bit Depth: %hhx, Color Type: %hhx, Compression Type: %hhx, Filter Method: %hhx, Interlace Method: %hhx\n\n", ihdr_chunk.width, ihdr_chunk.height, ihdr_chunk.bitDepth, ihdr_chunk.colorType, ihdr_chunk.compressionType, ihdr_chunk.filterMethod, ihdr_chunk.interlaceMethod);
                break;

            case 0x49454e44:
                printf("\tEnd Chunk!\n");
                break;
        }
    }

    printf("-------------------\n");
    
    free(original_file_content);
    return rgb_values;
}
