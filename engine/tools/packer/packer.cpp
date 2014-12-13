#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <contrib/minizip/zip.h>
#include <contrib/minizip/iostring.h>
#include <string>

/* Private functions */

//
// Displays program usage information
//
static
void usage()
{
	fprintf(stderr, "Usage: packer <base_path> <list_file> <out_file>\n");
	exit(1);
}


/* Public functions */

//
// Program entrypoint
//
int main(int argc, char ** argv)
{
	zlib_filefunc_def def;
	std::string result;

	if (argc != 4 || argv[1][0] == 0 || argv[1][0] == '-' || argv[2][0] == 0 || argv[2][0] == '-'
			|| argv[3][0] == 0 || argv[3][0] == '-')
		usage();

	fill_string_filefunc(&def, &result);
	zipFile zip = zipOpen2("packed", APPEND_STATUS_CREATE, NULL, &def);
	if (!zip)
	{
		fprintf(stderr, "Unable to create archive.\n");
		return 1;
	}	

	FILE * f = fopen(argv[2], "r");
	if (!f)
	{
		fprintf(stderr, "Unable to open file \"%s\": %s\n", argv[1], strerror(errno));
		return 1;
	}

	char buf[1024];
	while (fgets(buf, sizeof(buf), f))
	{
		int len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n')
			buf[--len] = 0;
		if (len == 0)
			continue;

		printf("Compressing %s\n", buf);

		std::string name = std::string(argv[1]) + "/" + buf;
		FILE * ff = fopen(name.c_str(), "rb");
		if (!ff)
		{
			fprintf(stderr, "Unable to open file \"%s\": %s\n", buf, strerror(errno));
			return 1;
		}
		fseek(ff, 0, SEEK_END);
		long size = ftell(ff);
		if (size < 0)
			size = 0;
		rewind(ff);
		std::string data(size, ' ');
		fread((char *)data.data(), 1, size, ff);
		fclose(ff);

		zip_fileinfo info;
		memset(&info, 0, sizeof(info));
		int code = zipOpenNewFileInZip(zip, buf, &info, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_BEST_COMPRESSION);
		if (code != ZIP_OK)
		{
			char err[64];

			switch (code)
			{
			case ZIP_ERRNO:
				strcpy(err, "ZIP_ERRNO");
				break;
			case ZIP_PARAMERROR:
				strcpy(err, "ZIP_PARAMERROR");
				break;
			case ZIP_BADZIPFILE:
				strcpy(err, "ZIP_BADZIPFILE");
				break;
			case ZIP_INTERNALERROR:
				strcpy(err, "ZIP_INTERNALERROR");
				break;
			default:
				sprintf(err, "code %d", code);
				break;
			}
			fprintf(stderr, "Unable to compress file \"%s\": %s\n", buf, err);
			fclose(f);
			return 1;
		}
		zipWriteInFileInZip(zip, data.data(), data.length());
		zipCloseFileInZip(zip);
	}

	fclose(f);

	zipClose(zip, NULL);

	f = fopen(argv[3], "w");
	if (!f)
	{
		fprintf(stderr, "Unable to create file \"%s\": %s\n", argv[2], strerror(errno));
		return 1;
	}

	fprintf(f, "\n");
	fprintf(f, "unsigned long zipDataLength = %d;\n", result.length());
	fprintf(f, "unsigned char zipData[] =\n");
	fprintf(f, "{\n");
	for (size_t i = 0; i < result.length(); i++)
	{
		if (i % 16 == 0)
		{
			if (i != 0)
				fprintf(f, "\n");
			fprintf(f, "\t");
		}
		else
			fprintf(f, " ");
		fprintf(f, "0x%02X,", (unsigned char)result[i]);
	}
	fprintf(f, "\n};\n");
	fprintf(f, "\n");

	fclose(f);

	return 0;
}
