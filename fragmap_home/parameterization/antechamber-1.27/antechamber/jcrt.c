/* GCRT */
int rjcrt(char *filename, int *atomnum, ATOM * atom, CONTROLINFO cinfo,
		  MOLINFO minfo)
{
	FILE *fpin;
	int numatom ;
	int overflow_flag = 0;
	int tmpint;
	int flag;
 	int i;
	char tmpchar[10];
	char tmpchar2[10];
	char line[MAXCHAR];


	if ((fpin = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Cannot open file %s, exit\n", filename);
		exit(1);
	}
	initial(cinfo.maxatom, atom, minfo.resname);
	numatom = 0;
	flag = 0;
	for (;;) {
		if (fgets(line, MAXCHAR, fpin) == NULL) {
/*     printf("\nFinished reading %s file.", cinfo.ifilename); */
			break;
		}
		sscanf(line, "%s", tmpchar);
		if(strncmp(tmpchar, "&zmat", 5)==0) {
			flag =1;
			continue;
		} 
		if(strncmp(tmpchar, "molchg=", 7)==0) {
			tmpint = 0;
			for(i=7;i<= strlen(tmpchar);i++)
				tmpchar2[tmpint++] = tmpchar[i];	
			minfo.icharge = atoi(tmpchar2);
			continue;
		}
		if(strncmp(tmpchar, "multip=", 7)==0) {
			tmpint = 0;
			for(i=7;i<= strlen(tmpchar);i++)
				tmpchar2[tmpint++] = tmpchar[i];	
			minfo.multiplicity = atoi(tmpchar2);
			continue;
		}

		if (flag == 1 && strcmp(tmpchar, "&") ==0) 
			break;

		if (overflow_flag == 0 && flag == 1) {
			sscanf(line, "%s%lf%lf%lf", atom[numatom].name,
				   &atom[numatom].x, &atom[numatom].y,
				   &atom[numatom].z);
			numatom++;
		}
		if (numatom >= cinfo.maxatom && overflow_flag == 0) {
			printf
				("\nInfo: the atom number exceeds the MAXATOM, reallocate memory automatically");
			overflow_flag = 1;
		}
	}
	*atomnum = numatom;
	atomicnum(*atomnum, atom);
	element(*atomnum, atom);

/* printf("\n atom number is  %5d", *atomnum); */
	fclose(fpin);
	return overflow_flag;
}

void wjcrt(char *filename, int atomnum, ATOM atom[], MOLINFO minfo)
{
	FILE *fpout;
	int i;
	/* int index; */

	if ((fpout = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "Cannot open file %s, exit\n", filename);
		exit(1);
	}
	fprintf(fpout, "%s\n", "This Jaguar input file (entry_name: %s) is generated by Antechamber", minfo.resname);
	fprintf(fpout, "%s\n", "&gen");
	fprintf(fpout, "%s%d\n", "molchg=", minfo.icharge);
	fprintf(fpout, "%s%d\n", "multip=", minfo.multiplicity);
	fprintf(fpout, "igeopt=1\n");
	fprintf(fpout, "basis=6-31G*\n");
	fprintf(fpout, "dft=b3lyp\n");
	fprintf(fpout, "%s\n", "&");
	fprintf(fpout, "%s\n", "&zmat");
	for (i = 0; i < atomnum; i++)
		fprintf(fpout, "%8s%12.8lf    %12.8lf    %12.8lf\n",
				atom[i].name, atom[i].x, atom[i].y, atom[i].z);

	fprintf(fpout, "%s\n", "&");
	fprintf(fpout, "\n");
	fclose(fpout);
}
