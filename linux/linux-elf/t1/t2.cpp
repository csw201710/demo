#include "t2.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

CPPUNIT_TEST_SUITE_REGISTRATION(SysTest);


SysTest::SysTest(){
}



void SysTest::setUp(){
	
	
}
void SysTest::tearDown(){
	
	
}
void SysTest::testService(){
	struct servent *sptr;
	sptr=getservbyname("echo","udp");
	printf("echo port=%d\n",ntohs(sptr->s_port));

	sptr=getservbyname("daytime","tcp");
	printf("daytime port=%d\n",ntohs(sptr->s_port));
	
	sptr=getservbyname("ftp","tcp");
	printf("ftp port=%d\n",ntohs(sptr->s_port));


	sptr=getservbyport(htons(7),"udp");
	printf("name=%s\n",sptr->s_name);

	sptr=getservbyport(htons(21),"tcp");
	printf("name=%s\n",sptr->s_name);
}


void SysTest::testPipe(){
	int fd[2]={0};
	int status=0;
	char buffer[1024]={0};
	pid_t pid = 0,_pid;
	if(pipe(fd) != 0){
		perror("pipe failed");
    return ;
	}
	pid = fork();
	if(pid < 0){
		perror("fork failed");
    return ;
	}
	if(pid == 0){
		//child
		const char * str="this is from child !";
		close(fd[0]);
		write(fd[1],str,strlen(str));
		exit(0);
	}
	close(fd[1]);
	read(fd[0],buffer,sizeof(buffer));
	printf("[message] %s\n",buffer);
	
	
	_pid = waitpid(pid,&status,0);
 	if(_pid==pid){
 		printf("Wait success for child\n");
 		printf("Exit code:%d,Exit signal:%d\n",(status>>8)&0xff,status&0xff); 
	}else{
		 perror("wait"); 
	}

}

#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


static void hexDump(const char* buf, int len)
{
	int i = 0;
	int leftSize = 0;
	int j = 0;
	const unsigned char *c = (const unsigned  char*)buf;
	if (len < 1 || buf == NULL) return;

	for (i = 0; i < (len / 16) * 16; i += 16)
	{
		
		printf("%08x  ",i);
		for (j = 0; j < 16; j++)
		{
			
			printf("%02x", c[i+j]);
			if((j+1)%8 == 0) printf("  "); 
			else printf(" "); 
			
		}
		printf("|");
		for (j = 0; j < 16; j++)
		{
			printf("%c", c[i+j] >= 32 && c[i+j] < 127 ? c[i+j] : '.');
		}
 		printf("|\n");
	}
 
	leftSize = len % 16;
	if (leftSize < 1) return;
		
	printf("%08x  ",i);
	for(j = 0; j< 16; j++){
		
		if(j < leftSize){
			printf("%02x", c[i+j]);
		}else{
			printf("  ");	
		}
		
		if((j+1)%8 == 0) printf("  ");
		else printf(" "); 
	}
	printf("|");	
	for(j = 0; j< 16; j++){
		if(j < leftSize){
			printf("%c", c[i+j] >= 32 && c[i+j] < 127 ? c[i+j] : '.');
		}else{
			printf(" ");	
		}
	}
	printf("|\n");
}


#define offsetof(TYPE, MEMBER) ((size_t) &(((TYPE*)0)->MEMBER))


	


#define CASE_EQUAL(t1,t2) \
	case t1: {str = t2; break; }
		
static const char * get_e_type_info(Elf64_Half type){
	const char* str = "";
	switch(type){
		CASE_EQUAL(ET_NONE,"No file type");
		CASE_EQUAL(ET_REL,"Relocatable file");
		CASE_EQUAL(ET_EXEC,"Executable file");
		CASE_EQUAL(ET_DYN,"Shared object file");
		CASE_EQUAL(ET_CORE,"Core file");
		CASE_EQUAL(ET_NUM,"Number of defined types");
		CASE_EQUAL(ET_LOOS,"OS-specific range start");
		CASE_EQUAL(ET_HIOS,"OS-specific range end");
		CASE_EQUAL(ET_LOPROC,"Processor-specific range start");
		CASE_EQUAL(ET_HIPROC,"Processor-specific range end");
	}
	return str;
}

static const char * get_e_machine_info(Elf64_Half type){
	const char* str = "";
	switch(type){
		CASE_EQUAL(EM_NONE,"No machine");
		CASE_EQUAL(EM_M32,"AT&T WE 32100");
		CASE_EQUAL(EM_SPARC,"SUN SPARC");
		CASE_EQUAL(EM_386,"Intel 80386");
		CASE_EQUAL(EM_68K,"Motorola m68k family");
		CASE_EQUAL(EM_88K,"Motorola m88k family");
		CASE_EQUAL(EM_860,"Intel 80860");
		CASE_EQUAL(EM_MIPS,"MIPS R3000 big-endian");
		CASE_EQUAL(EM_S370,"IBM System/370");
		CASE_EQUAL(EM_MIPS_RS3_LE,"MIPS R3000 little-endian");

		CASE_EQUAL(EM_PARISC,"HPPA");
		CASE_EQUAL(EM_VPP500,"Fujitsu VPP500");
		CASE_EQUAL(EM_SPARC32PLUS,"Sun's \"v8plus\"");
		CASE_EQUAL(EM_960,"Intel 80960");
		CASE_EQUAL(EM_PPC,"PowerPC");
		CASE_EQUAL(EM_PPC64,"PowerPC 64-bit");
		CASE_EQUAL(EM_S390,"IBM S390");

		CASE_EQUAL(EM_V800,"NEC V800 series");
		CASE_EQUAL(EM_FR20,"Fujitsu FR20");
		CASE_EQUAL(EM_RH32,"TRW RH-32");
		CASE_EQUAL(EM_RCE,"Motorola RCE");
		CASE_EQUAL(EM_ARM,"ARM");
		CASE_EQUAL(EM_FAKE_ALPHA,"Digital Alpha");
		CASE_EQUAL(EM_SH,"Hitachi SH");
		CASE_EQUAL(EM_SPARCV9,"SPARC v9 64-bit");
		CASE_EQUAL(EM_TRICORE,"Siemens Tricore");
		CASE_EQUAL(EM_ARC,"Argonaut RISC Core");
		CASE_EQUAL(EM_H8_300,"Hitachi H8/300");
		CASE_EQUAL(EM_H8_300H,"Hitachi H8/300H");
		CASE_EQUAL(EM_H8S,"Hitachi H8S");
		CASE_EQUAL(EM_H8_500,"Hitachi H8/500");
		CASE_EQUAL(EM_IA_64,"Intel Merced");
		CASE_EQUAL(EM_MIPS_X,"Stanford MIPS-X");
		CASE_EQUAL(EM_COLDFIRE,"Motorola Coldfire");
		CASE_EQUAL(EM_68HC12,"Motorola M68HC12");
		CASE_EQUAL(EM_MMA,"Fujitsu MMA Multimedia Accelerator");
		CASE_EQUAL(EM_PCP,"Siemens PCP");
		CASE_EQUAL(EM_NCPU,"Sony nCPU embeeded RISC");
		CASE_EQUAL(EM_NDR1,"Denso NDR1 microprocessor");
		CASE_EQUAL(EM_STARCORE,"Motorola Start*Core processor");
		CASE_EQUAL(EM_ME16,"Toyota ME16 processor");
		CASE_EQUAL(EM_ST100,"STMicroelectronic ST100 processor");
		CASE_EQUAL(EM_TINYJ,"Advanced Logic Corp. Tinyj emb.fam");
		CASE_EQUAL(EM_X86_64,"AMD x86-64 architecture");
		CASE_EQUAL(EM_PDSP,"Sony DSP Processor");

		CASE_EQUAL(EM_FX66,"Siemens FX66 microcontroller");
		CASE_EQUAL(EM_ST9PLUS,"STMicroelectronics ST9+ 8/16 mc");
		CASE_EQUAL(EM_ST7,"STmicroelectronics ST7 8 bit mc");
		CASE_EQUAL(EM_68HC16,"Motorola MC68HC16 microcontroller");
		CASE_EQUAL(EM_68HC11,"Motorola MC68HC11 microcontroller");
		CASE_EQUAL(EM_68HC08,"Motorola MC68HC08 microcontroller");
		CASE_EQUAL(EM_68HC05,"Motorola MC68HC05 microcontroller");
		CASE_EQUAL(EM_SVX,"Silicon Graphics SVx");
		CASE_EQUAL(EM_ST19,"STMicroelectronics ST19 8 bit mc");
		CASE_EQUAL(EM_VAX,"Digital VAX");
		CASE_EQUAL(EM_CRIS,"Axis Communications 32-bit embedded processor");
		CASE_EQUAL(EM_JAVELIN,"Infineon Technologies 32-bit embedded processor");
		CASE_EQUAL(EM_FIREPATH,"Element 14 64-bit DSP Processor");
		CASE_EQUAL(EM_ZSP,"LSI Logic 16-bit DSP Processor");
		CASE_EQUAL(EM_MMIX,"Donald Knuth's educational 64-bit processor");
		CASE_EQUAL(EM_HUANY,"Harvard University machine-independent object files");
		CASE_EQUAL(EM_PRISM,"SiTera Prism");
		CASE_EQUAL(EM_AVR,"Atmel AVR 8-bit microcontroller");
		CASE_EQUAL(EM_FR30,"Fujitsu FR30");
		CASE_EQUAL(EM_D10V,"Mitsubishi D10V");
		CASE_EQUAL(EM_D30V,"Mitsubishi D30V");
		CASE_EQUAL(EM_V850,"NEC v850");
		CASE_EQUAL(EM_M32R,"Mitsubishi M32R");
		CASE_EQUAL(EM_MN10300,"Matsushita MN10300");
		CASE_EQUAL(EM_MN10200,"Matsushita MN10200");
		CASE_EQUAL(EM_PJ,"picoJava");
		CASE_EQUAL(EM_OPENRISC,"OpenRISC 32-bit embedded processor");
		CASE_EQUAL(EM_ARC_A5,"ARC Cores Tangent-A5");
		CASE_EQUAL(EM_XTENSA,"Tensilica Xtensa Architecture");
		CASE_EQUAL(EM_AARCH64,"ARM AARCH64");
		CASE_EQUAL(EM_TILEPRO,"Tilera TILEPro");
		CASE_EQUAL(EM_TILEGX,"Tilera TILE-Gx");
	}
	return str;
}

static const char * get_e_flags_info(Elf64_Word  type){
	const char* str = "";
	switch(type){
		CASE_EQUAL(EF_SPARCV9_MM,"内存型号掩码");
		CASE_EQUAL(EF_SPARCV9_TSO,"总体存储排序");
		CASE_EQUAL(EF_SPARCV9_PSO,"部分存储排序");
		CASE_EQUAL(EF_SPARCV9_RMO,"非严格存储排序");
		CASE_EQUAL(EF_SPARC_LEDATA,"little endian data");
		CASE_EQUAL(EF_SPARC_EXT_MASK,"供应商扩展掩码 ");
		CASE_EQUAL(EF_SPARC_32PLUS,"generic V8+ features");
		CASE_EQUAL(EF_SPARC_SUN_US1,"Sun UltraSPARC1 extensions");
		CASE_EQUAL(EF_SPARC_HAL_R1,"HAL R1 extensions");
		CASE_EQUAL(EF_SPARC_SUN_US3,"Sun UltraSPARCIII extensions");
	}
	return str;
}

#define PRINT_ELF64HR_M(value,info) \
	printf("Elf64_Ehdr " #value " : %#x (%s)",value,info );


void printElfHeader(size_t offset,Elf64_Ehdr * p){
	switch(offset){
		case offsetof(Elf64_Ehdr,e_ident):{
			printf("Elf64_Ehdr.e_ident: \n\t");
			hexDump((const char*)p->e_ident,sizeof(p->e_ident));
			break;
		}
		case offsetof(Elf64_Ehdr,e_type):{
			PRINT_ELF64HR_M(p->e_type,get_e_type_info(p->e_type));
			break;
		}
		case offsetof(Elf64_Ehdr,e_machine):{
			PRINT_ELF64HR_M(p->e_machine,get_e_machine_info(p->e_machine));
			break;
		}
		case offsetof(Elf64_Ehdr,e_version):{
			PRINT_ELF64HR_M(p->e_version, p->e_version == 0 ? "Invalid ELF version":"Current version");
			break;
		}
		case offsetof(Elf64_Ehdr,e_entry):{
			PRINT_ELF64HR_M(p->e_entry, "Entry point virtual address");
			break;
		}
		case offsetof(Elf64_Ehdr,e_phoff):{
			PRINT_ELF64HR_M(p->e_phoff, "Program header table file offset");
			break;
		}
		case offsetof(Elf64_Ehdr,e_shoff):{
			PRINT_ELF64HR_M(p->e_shoff, "Section header table file offset");
			break;
		}
		case offsetof(Elf64_Ehdr,e_flags):{
			PRINT_ELF64HR_M(p->e_flags, get_e_flags_info(p->e_flags));
			printf(" 与文件关联的特定于处理器的标志");
			break;
		}
		case offsetof(Elf64_Ehdr,e_ehsize):{
			PRINT_ELF64HR_M(p->e_ehsize, "ELF header size in bytes");
			break;
		}
		case offsetof(Elf64_Ehdr,e_phentsize):{
			PRINT_ELF64HR_M(p->e_phentsize, "Program header table entry size");
			printf(" 所有项的大小都相同");
			break;
		}
		case offsetof(Elf64_Ehdr,e_phnum):{
			PRINT_ELF64HR_M(p->e_phnum, "Program header table entry count");
			break;
		}
		case offsetof(Elf64_Ehdr,e_shentsize):{
			PRINT_ELF64HR_M(p->e_shentsize, "Section header table entry size");
			break;
		}
		case offsetof(Elf64_Ehdr,e_shnum):{
			PRINT_ELF64HR_M(p->e_shnum, "Section header table entry count");
			break;
		}
		case offsetof(Elf64_Ehdr,e_shstrndx):{
			PRINT_ELF64HR_M(p->e_shstrndx, "Section header string table index");
			break;
		}
		
		default:
			return;
	}
	printf("\n");
}


void SysTest::parseElf(){
	
	int fd=open("/root/test/1",O_RDONLY );
	assert(fd != -1);
	
	struct stat st; //定义文件信息结构体
  /*取得文件大小*/
  int r=fstat(fd,&st);
  assert(r != -1);
  /*把文件映射成虚拟内存地址*/
  char *p=(char*)mmap(NULL,st.st_size,PROT_READ,MAP_SHARED,fd,0);    
  assert(p != 0);
  Elf64_Ehdr * elf_hdr= (Elf64_Ehdr*)p;
  
 	printElfHeader(offsetof(Elf64_Ehdr,e_ident),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_type),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_machine),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_version),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_entry),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_phoff),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_shoff),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_flags),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_ehsize),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_phentsize),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_phnum),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_shentsize),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_shnum),elf_hdr);
 	printElfHeader(offsetof(Elf64_Ehdr,e_shstrndx),elf_hdr);
  

  munmap(p,st.st_size);
  close(fd);
}