#include<stdio.h>
#include<io.h>
#include<direct.h>
#include<stdlib.h>
#include<string.h>


void pack()
{
	printf("\npack���÷�:\n\n");
	printf("\t-t\t���һ���ļ�\n");
	printf("\t-g\t��ѹһ������ļ�\n");
	printf("\t-l\t�鿴����ļ���Ϣ\n");
	printf("\t-r\t���ƥ���ļ�\n\n");
	return;
}

typedef struct file {
	char name[_MAX_FNAME];
	_fsize_t size;
	int bol;
} file;

//�������pack_t()
//������Ҫ��� E:\temp �ļ��У��ṩ���� E:\temp������ִ�к��ɾ��temp�ļ��У�����һ����Ϊtemp1�Ĵ���ļ�, ����λ����E:\temp1
_fsize_t pack_t(char *s1)
{
	char folder_location[_MAX_FNAME]; //�洢�ļ���·��
	strcpy_s(folder_location, s1);
	strcat_s(folder_location, "\\*");
	struct _finddata_t file_info;
	long handle = _findfirst(folder_location, &file_info);
	if (handle == -1) {
		printf("folder location ERROR!");
		return 0;
	}

	int file_sum = 0; //�洢�ļ����ļ�������
	do {
		if (strcmp(file_info.name, ".") != 0 && strcmp(file_info.name, "..") != 0)
			++file_sum;
	} while (_findnext(handle, &file_info) == 0); //ͳ���ļ����ļ������� ���� . �� .. �ļ�
	_findclose(handle);

	char file_location[_MAX_FNAME];          //����ļ���Ϣ
	strcpy_s(file_location, folder_location);
	int len2 = strlen(file_location);
	file_location[len2 - 2] = '\0';
	strcat_s(file_location, "1");     //���������ļ����ļ���
	//FILE *in = fopen(file_location, "ab");
	FILE *in;
	fopen_s(&in, file_location, "ab");

	char delete_folder_string[_MAX_FNAME] = "rd ";   //ɾ���ļ����ַ���
	strcat_s(delete_folder_string, folder_location);
	int len = strlen(delete_folder_string);
	delete_folder_string[len - 2] = '\0';

	if (file_sum == 0) {
		fwrite(&file_sum, sizeof(int), 1, in);
		fclose(in);
		system(delete_folder_string);      //ɾ���ļ���
		return sizeof(int);
	}

	int i = 0;
	file *file_info_array = (file *)malloc(file_sum * sizeof(file));  //��¼ÿ���ļ���Ϣ
	handle = _findfirst(folder_location, &file_info);
	do {
		if (strcmp(file_info.name, ".") != 0 && strcmp(file_info.name, "..") != 0) {
			if (file_info.attrib == _A_SUBDIR) {
				char folder_folder_location[_MAX_FNAME];  //�洢�ļ��е�·��
				strcpy_s(folder_folder_location, folder_location);
				int len1 = strlen(folder_folder_location);
				folder_folder_location[len1 - 1] = '\0';
				strcat_s(folder_folder_location, file_info.name);

				strcpy_s(file_info_array[i].name, file_info.name);
				strcat_s(file_info_array[i].name, "1");
				file_info_array[i].size = pack_t(folder_folder_location);  //�ݹ�����ļ�����
				file_info_array[i].bol = 1;               //1��ʾ�ļ���
				++i;
			}
			else {
				strcpy_s(file_info_array[i].name, file_info.name);
				file_info_array[i].size = file_info.size;
				file_info_array[i].bol = 0;              //0��ʾ�ļ�
				++i;
			}
		}

	} while (_findnext(handle, &file_info) == 0);
	_findclose(handle);

	_fsize_t ret = 0;
	fwrite(&file_sum, sizeof(int), 1, in);
	ret += sizeof(int) * 1;
	fwrite(file_info_array, sizeof(file), file_sum, in);
	ret += sizeof(file) * file_sum;

	for (i = 0; i < file_sum; ++i) {             //copy�ļ�
		char folder_file_location[_MAX_FNAME];    //�洢�ļ������ļ���λ��
		strcpy_s(folder_file_location, folder_location);
		folder_file_location[len2 - 1] = '\0';
		strcat_s(folder_file_location, file_info_array[i].name);
		//FILE *out = fopen(folder_file_location, "rb");
		FILE *out;
		fopen_s(&out, folder_file_location, "rb");

		int rc = 0;  //ÿһ��ת�Ƶ��ֽ���
		unsigned char *buf = (unsigned char *)malloc(30 * sizeof(unsigned char)); //�洢ÿһ��ת�Ƶ��ֽ�
		while ((rc = fread(buf, sizeof(unsigned char), 30, out)) != 0) {
			ret += rc;
			fwrite(buf, sizeof(unsigned char), rc, in);
		}

		free(buf);
		fclose(out);
		//remove(folder_file_location);         //ɾ���ļ�!!!!!!!!!!!
	}

	free(file_info_array);
	fclose(in);

	//system(delete_folder_string);      //ɾ���ļ���!!!!!!!!!!!

	return ret;
}

//�鿴����ļ���Ϣpack_l()
//�ṩ����ļ���λ�ã�����E:\temp1
typedef struct folder_info {
	char str[_MAX_FNAME];
	int begin;
	int end;
	long int offset;
	struct folder_info *next;
} folder;

void pack_l(char *s1)
{
	char file_location[_MAX_FNAME];
	strcpy_s(file_location, s1);
	int len = strlen(file_location) - 1;
	file_location[len] = '\0';  //ȥ��'1'
	while (len >= 0 && file_location[len] != '\\')
		--len;
	++len;
	printf("\n%s : \n", file_location + len);

	//FILE *out = fopen(s1, "rb");
	FILE *out;
	fopen_s(&out, s1, "rb");
	folder *hand = (folder *)malloc(sizeof(folder));
	strcpy_s(hand->str, "\t");
	hand->begin = 0;
	hand->offset = 0;
	hand->next = NULL;

	while (hand != NULL) {
		fseek(out, hand->offset, SEEK_SET);
		int offset = hand->offset;
		int file_sum = 0;
		fread(&file_sum, sizeof(int), 1, out);
		offset += sizeof(int);
		hand->end = file_sum;

		if (file_sum != 0) {
			file *file_info_array = (file *)malloc(file_sum * sizeof(file));  //��¼ÿ���ļ���Ϣ
			fread(file_info_array, sizeof(file), file_sum, out);
			offset += sizeof(file) * file_sum;

			int i = 0;
			for (i = 0; i < hand->begin; ++i)
				offset += file_info_array[i].size;
			for (i = hand->begin; i < hand->end; ++i) {
				if (file_info_array[i].bol == 0)        //�ļ�
					printf("%s%s\n", hand->str, file_info_array[i].name, file_info_array[i].size);
				else {            //�ļ���
					char folder_name[_MAX_FNAME];
					strcpy_s(folder_name, file_info_array[i].name);
					folder_name[strlen(folder_name) - 1] = '\0';
					printf("%s%s : \n", hand->str, folder_name);
					hand->begin = i + 1;

					folder *last = (folder *)malloc(sizeof(folder));
					last->begin = 0;
					last->end = -1;
					last->next = hand;
					last->offset = offset;
					strcpy_s(last->str, hand->str);
					strcat_s(last->str, "\t");
					hand = last;

					break;
				}
				offset += file_info_array[i].size;
			}
			free(file_info_array);
			if (i == hand->end) {
				folder* next = hand->next;
				free(hand);
				hand = next;
			}
		}
		else {      //file_sum == 0
			folder* next = hand->next;
			free(hand);
			hand = next;
		}
	}

	fclose(out);
	printf("\n");
	return;
}

//�������pack_g()
//�ṩ����ļ���λ�ã�����E:\temp1
void pack_g(char *s1)
{
	char pack_file[_MAX_FNAME];
	strcpy_s(pack_file, s1);

	int len = strlen(pack_file);
	pack_file[len - 1] = '\0';   //ȥ1!!!!!!!!!!!
	char create_folder_string[_MAX_FNAME] = "md ";  //�洢��Ҫ�������ļ��е�·��
	strcat_s(create_folder_string, pack_file);
	system(create_folder_string);  //�����ļ���

	int file_sum;
	//FILE *out = fopen(s1, "rb");
	FILE *out;
	fopen_s(&out, s1, "rb");
	fread(&file_sum, sizeof(int), 1, out);
	if (file_sum == 0) {      //���ļ��д���ɵ��ļ�
		fclose(out);         //�ر��ļ���
		remove(s1);          //ɾ������ļ�
		return;
	}
	file *file_info_array = (file *)malloc(file_sum * sizeof(file));  //��¼ÿ���ļ���Ϣ
	fread(file_info_array, sizeof(file), file_sum, out);

	for (int i = 0; i < file_sum; ++i) {     //���ν�ѹ�����ļ�
		char file_location[_MAX_FNAME];
		strcpy_s(file_location, pack_file);
		strcat_s(file_location, "\\");
		strcat_s(file_location, file_info_array[i].name);
		//FILE *in = fopen(file_location, "wb");  //�����ļ�
		FILE *in;
		fopen_s(&in, file_location, "wb");

		int rc = 0;  //ÿһ��ת�Ƶ��ֽ���
		unsigned char *buf = (unsigned char *)malloc(30 * sizeof(unsigned char)); //�洢ÿһ��ת�Ƶ��ֽ�
		int size = file_info_array[i].size;  //�ļ���С
		while (size != 0) {
			rc = size >= 30 ? 30 : size;
			size -= rc;
			fread(buf, sizeof(unsigned char), rc, out);
			fwrite(buf, sizeof(unsigned char), rc, in);
		}

		free(buf);
		fclose(in);

		if (file_info_array[i].bol == 1) {     //�����ѹ�����ļ���һ��������ļ��� ��
			pack_g(file_location);
		}
	}

	free(file_info_array);
	fclose(out);
	//remove(s1);              //ɾ������ļ�!!!!!!!!!!!!!!!
	return;
}

int match(char *s1, char *s2)
{
	if (*s1 == '\0' && *s2 == '\0')
		return 1;
	if (*s2 == '\0')
		return 0;
	if (*s1 == '\0' && *s2 != '*')
		return 0;
	if (*s1 == '\0' && *s2 == '*')
		match(s1, s2 + 1);
	if (*s2 == '?')
		return match(s1 + 1, s2 + 1);
	if (*s2 == '*')
		return match(s1, s2 + 1) || match(s1 + 1, s2);
}

//���ƥ���ļ�pack_r()
//�ṩ������������һ����ƥ���ļ�λ�ü�ƥ����Ϣ������E:\temp*.txt ���ڶ����Ǵ���ɵ��ļ���λ�ã�����E : \one�����������û��дû�ж�Ӧ�Ľ��������
void pack_r(char *s1, char *s2)
{
	char match_string[_MAX_FNAME];
	char folder_location[_MAX_FNAME];
	int len = strlen(s1);
	while (len >= 0 && s1[len] != '\\')
		--len;
	strncpy_s(folder_location, s1, len);
	folder_location[len] = '\0';
	char match_file_location[_MAX_FNAME];
	strcpy_s(match_file_location, folder_location);
	strcat_s(folder_location, "\\*");
	++len;
	strcpy_s(match_string, s1 + len);
	char file_location[_MAX_FNAME];
	strcpy_s(file_location, s2);

	struct _finddata_t file_info;
	long handle = _findfirst(folder_location, &file_info);
	if (handle == -1) {
		printf("folder location ERROR!");
		return;
	}
	int file_sum = 0; //ƥ���ļ�����
	do {
		if (file_info.attrib != _A_SUBDIR && match(file_info.name, match_string))
			++file_sum;
	} while (_findnext(handle, &file_info) == 0); //ͳ��ƥ���ļ�����
	_findclose(handle);
	if (file_sum == 0)
		printf("match string ERROR!");

	int i = 0;
	file *file_info_array = (file *)malloc(file_sum * sizeof(file));  //��¼ÿ���ļ���Ϣ
	handle = _findfirst(folder_location, &file_info);
	do {
		if (file_info.attrib != _A_SUBDIR && match(file_info.name, match_string)) {
			file_info_array[i].bol = 1;
			file_info_array[i].size = file_info.size;
			strcpy_s(file_info_array[i].name, file_info.name);
			++i;
		}
	} while (_findnext(handle, &file_info) == 0);
	_findclose(handle);

	//FILE *in = fopen(file_location, "wb");
	FILE *in;
	fopen_s(&in, file_location, "wb");
	if (in == NULL) {
		free(file_info_array);
		printf("pack file location ERROR!");
		return;
	}
	fwrite(&file_sum, sizeof(int), 1, in);
	fwrite(file_info_array, sizeof(file), file_sum, in);

	strcat_s(match_file_location, "\\");
	for (i = 0; i < file_sum; ++i) {
		char str[_MAX_FNAME];
		strcpy_s(str, match_file_location);
		strcat_s(str, file_info_array[i].name);
		//FILE *out = fopen(str, "rb");
		FILE *out;
		fopen_s(&out, str, "rb");

		int rc = 0;  //ÿһ��ת�Ƶ��ֽ���
		unsigned char *buf = (unsigned char *)malloc(30 * sizeof(unsigned char)); //�洢ÿһ��ת�Ƶ��ֽ�
		while ((rc = fread(buf, sizeof(unsigned char), 30, out)) != 0) {
			fwrite(buf, sizeof(unsigned char), rc, in);
		}
		free(buf);
		fclose(out);
	}

	free(file_info_array);
	fclose(in);

	return;
}


//int main(int argc, char *argv[])
//int main()
//{
	//���pack_t()���鿴����ļ���Ϣpack_l()���������pack_g()��
	//���ƥ���ļ�pack_r()
	//�ṩ������������һ����ƥ���ļ�λ�ü�ƥ����Ϣ������E:\temp*.txt ���ڶ����Ǵ���ɵ��ļ���λ�ã�����E : \one�����������û��дû�ж�Ӧ�Ľ��������
	/*int d=0;
	printf("������d��");
	scanf_s("%d", d);
	if (d == 0)
		pack();
	else if (d == 1) {
		printf("����ļ���������·����");
		char path_t[50];
		scanf_s("%c", &path_t);
		pack_t(path_t);//���
	}
	else if (d == 2) {
		printf("�鿴����ļ���Ϣ��������·����");
		char path_l[50];
		scanf_s("%c", &path_l);
		pack_l(path_l);//�������Ϣ
	}
	else if (d == 3) {
		printf("����ļ���������·����");
		char path_g[50];
		scanf_s("%c", &path_g);
		pack_g(path_g);//���
	}

	printf("=====");
	if (argc == 1 && strcmp(argv[0], "pack") == 0)
		pack();
	if (argc == 3 && strcmp(argv[1], "-t") == 0)
		pack_t(argv[2]);
	if (argc == 4 && strcmp(argv[1], "-r") == 0)
		pack_r(argv[2], argv[3]);
	if (argc == 3 && strcmp(argv[1], "-g") == 0)
		pack_g(argv[2]);
	if (argc == 3 && strcmp(argv[1], "-l") == 0)
		pack_l(argv[2]);

	system("pause");
	return 0;
}*/

int main()
{
	//���pack_t()���鿴����ļ���Ϣpack_l()���������pack_g()��
	//���ƥ���ļ�pack_r()
	//�ṩ������������һ����ƥ���ļ�λ�ü�ƥ����Ϣ������E:\temp*.txt ���ڶ����Ǵ���ɵ��ļ���λ�ã�����E : \one�����������û��дû�ж�Ӧ�Ľ��������
	int d=0;
	printf("������d��");
	scanf_s("%d", &d);
	if (d == 0)
		pack();
	else if (d == 1) {
		printf("����ļ���������·����");
		//char path_t[50];
		//scanf_s("%c",&path_t);
		//char *fp_t = path_t;

		//char *name0;
		//scanf_s("%s", &name0);
		//pack_t(name0);//���
		//char *s = (char *)malloc(50);//����nΪҪ���ٿռ�Ĵ�С
		char name11[] = "E:\AAAsource";
		char* name11_fp = name11;

		/*const char* buf1 = "E:\AAAsource";

		char img2[] = "D:/src.jpg";
		char* image_filename2 = img2;
		char img1[] = "D:/Demo1.jpg";
		char* image_filename1 = img1;*/

		pack_t(name11_fp);
		printf("����ɹ�\n");
		
		//char name22[] = "E:\AAAsource";
		//char* name22_fp = name22;
		//pack_l(name22_fp);
		//printf("����ɹ�========");
		//printf("����ļ���������·����");
		//char name33[] = "E:\AAAsource\c1";
		//char* name33_fp = name33;
		//pack_g(name33_fp);
		//printf("����ɹ�");
	}
	else if (d == 2) {
		printf("����ļ���������·����");
		char name33[] = "E:\AAAsource1";
		char* name33_fp = name33;
		pack_g(name33_fp);
		printf("����ɹ�");
	}
	else if (d == 3) {
		printf("����ļ���������·����");
		char name11[] = "E:\AAAsource\a.txt";
		char* name11_fp = name11;


		printf("����ļ���������Ŀ�ĵ�·����");
		char name33[] = "E:\AAAsource\c";
		char* name33_fp = name33;
		pack_r(name11_fp, name33_fp);
		printf("����ɹ�\n");
	}
	system("pause");
	return 0;
}