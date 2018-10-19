/*
 * File: main.c
 *
 * Arquivo principal do aplicativo teditor.bin 
 * O aplicativo é usado para testes do sistema operacional Gramado 0.3.
 *
 * 2018 - Created by Fred Nora.
 */


#include <types.h>

#include "heap.h"
#include "api.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "status.h"
#include "topbar.h"


//#define TEDITOR_VERBOSE 1



//static int running = 1;
int running = 1;

//static char *dest_argv[] = { "-sujo0","-sujo1","-sujo2",NULL };
//static unsigned char *dest_envp[] = { "-sujo", NULL };
//static unsigned char dest_msg[512];

void editorClearScreen(); 
int editor_save_file ();

/*
 ************************************************************
 * mainGetMessage:
 *     Função principla chamada pelo crt0.asm.
 *     Testando o recebimento de mensagens enviadas pelo shell.
 */
 
#define LSH_TOK_DELIM " \t\r\n\a" 
#define SPACE " "
#define TOKENLIST_MAX_DEFAULT 80
 
int mainGetMessage (){
	
	char *tokenList[TOKENLIST_MAX_DEFAULT];
	char *token;
	int token_count;
	int index;	
	
	
	// #importante
	// Linha de comandos passada pelo shell.
	char *shared_memory = (char *) (0xC0800000 -0x100);	
	
	
#ifdef TEDITOR_VERBOSE	
	
	printf("\n");
	printf("mainGetMessage:\n");
	printf("Initializing teditor.bin ...\n\n");	
	//printf("\n");
	//printf(".\n");
	printf("..\n");
	printf("# MESSAGE={%s} #\n", shared_memory );
	printf("..\n");
	//printf(".\n");
	//printf("\n");
	
	//#debug
	//while(1){
	//	asm ("pause");
	//}
	
#endif
	

    // Criando o ambiente.
	// Transferindo os ponteiros do vetor para o ambiente.

	tokenList[0] = strtok ( &shared_memory[0], LSH_TOK_DELIM );
	
 	// Salva a primeira palavra digitada.
	token = (char *) tokenList[0];
 
	index = 0;                                  
    while ( token != NULL )
	{
        // Coloca na lista.
        // Salva a primeira palavra digitada.
		tokenList[index] = token;

		//#debug
        //printf("shellCompare: %s \n", tokenList[i] );
		
		token = strtok ( NULL, LSH_TOK_DELIM );
		
		// Incrementa o índice da lista
        index++;
		
		// Salvando a contagem.
		token_count = index;
    }; 

	//Finalizando a lista.
    tokenList[index] = NULL;	
	
	
	// #debug 
	// Mostra argumentos.
#ifdef TEDITOR_VERBOSE	
	// Mostra a quantidade de argumentos. 	
	printf("\n");
	printf("token_count={%d}\n", token_count );
	
	//Mostra os primeiros argumentos.
	for ( index=0; index < token_count; index++ )
	{
		token = (char *) tokenList[index];
	    if ( token == NULL )
		{
			printf("mainGetMessage: for fail!\n")
			goto hang;
		}
	    printf("# argv{%d}={%s} #\n", index, tokenList[index] );		
	};
#endif	
	
	

	
#ifdef TEDITOR_VERBOSE		
    //Inicializando o editor propriamente dito.	
	printf("Calling mainTextEditor ... \n"); 
#endif	
	
	if ( mainTextEditor( token_count, tokenList ) == 1 )
	{
#ifdef TEDITOR_VERBOSE				
		printf("mainGetMessage: mainTextEditor returned 1.\n");
#endif 
	}else{
//#ifdef TEDITOR_VERBOSE				
		//printf("mainGetMessage: mainTextEditor returned 0.\n");
//#endif
	};
	
    printf("*HANG\n");
	
    while (1){
        asm("pause");
    };
};


/*
 ********************************************
 * mainTextEditor:
 *     O editor de textos.
 * In this fuction:
 *     Initializes crt.
 *     Initializes stdio.
 *
 */
int mainTextEditor ( int argc, char *argv[] ){
	
	int ch;
	FILE *fp;
    int char_count = 0;	
	
	struct window_d *hWindow;
	
	
	//#todo: analizar a contagem de argumentos.
	
	
#ifdef TEDITOR_VERBOSE			
	printf("\n");
	printf("Initializing Text Editor:\n");
	printf("mainTextEditor: # argv0={%s} # \n", argv[0] );	
	printf("mainTextEditor: # argv1={%s} # \n", argv[1] );
#endif	

    //#debug
    //while(1){
	//	asm ("pause");
	//}
	
	//
	// ## vamos repetir o que dá certo ...
	//
	
	//vamos passar mais coisas via registrador.
	
	//ok
	//foi passado ao crt0 via registrador
	//printf("argc={%d}\n", argc ); 
	
	//foi passado ao crt0 via memória compartilhada.
	//printf("argvAddress={%x}\n", &argv[0] ); //endereço.
	
	
	//unsigned char* buf = (unsigned char*) (0x401000 - 0x100) ;
	//printf("argvString={%s}\n" ,  &argv[0] );
	//printf("argvString={%s}\n" , &buf[0] );
	
	//printf("argv={%s}\n", &argv[2] );
	
	
    //stdlib
	//inicializando o suporte a alocação dinâmica de memória.
	libcInitRT();

	//stdio
	//inicializando o suporte ao fluxo padrão.
    stdioInitialize();	
	

	//
	// ## Window ##
	//
    
	//Criando uma janela para meu editor de textos.
	apiBeginPaint(); 
	
	//hWindow = (void *) APICreateWindow ( WT_OVERLAPPED, 1, 1,"TEDITOR",
	//                    20, 20, 800-40, 600-40,    
    //                    0, 0, 0x303030, 0x303030 );

	hWindow = (void *) APICreateWindow ( WT_OVERLAPPED, 1, 1, argv[1],
	                    20, 20, 800-40, 600-40,    
                        0, 0, 0x303030, 0x303030 );	   
	

	if ( (void *) hWindow == NULL )
	{	
		printf("TEDITOR.BIN: hWindow fail");
		apiEndPaint();
		goto fail;
	}
    apiEndPaint();
	
    APIRegisterWindow(hWindow);
    APISetActiveWindow(hWindow);	
    APISetFocus(hWindow);	
	refresh_screen ();	


	//apiBeginPaint();    
	//editorClearScreen(); 
	//topbarInitializeTopBar();
	//statusInitializeStatusBar();
	//update_statuts_bar("# Status bar string 1","# Status bar string 2");
	//apiEndPaint();
	
	
	//apiBeginPaint();
	//?? edit box ??
	//apiEndPaint();
	
	//
	//  ## Testing file support. ##
	//
	
//file:

#ifdef TEDITOR_VERBOSE		
	printf("\n");
	printf("\n");
    printf("Loading file ...\n");
#endif	
	
	//Page fault:
    
	// Pegando o argumento 1, porque o argumento 0 é o nome do editor.
	
	//#test
	//Se nenhum nome de arquivo foi especificado, então começamos a digitar.
	if ( (char *) argv[1] == NULL )
	{	
	    goto startTyping;	
	};
	
	//Carregando arquivo.
	fp = fopen ( (char *) argv[1], "rb" );	
	
	if ( fp == NULL )
    {
        printf("fopen fail\n");
        goto startTyping;
		
    }else{
		
		//Mostrando o arquivo.
		
#ifdef TEDITOR_VERBOSE			
        printf(".\n");		
        printf("..\n");		
        //printf("...\n");
#endif 
	
        printf ( "%s", fp->_base );	

#ifdef TEDITOR_VERBOSE	        
		//printf("...\n");
        printf("..\n");		
        printf(".\n");		
#endif

startTyping:

#ifdef TEDITOR_VERBOSE	
		printf("\n");
		printf("Typing a text ...\n");
#endif


//#importante:
//Esse loop deve ser um loop de mensagens 
//e não de chars. Quem tem loop de chars 
//é message box.
//Pois o aplicativo deve receber mensagens 
//sobre eventos de input de teclado e mouse,
//assim como os controles.

Mainloop:	

	//
	// Habilitando o cursor de textos.
	//
	
	system_call ( 244, (unsigned long) 0, (unsigned long) 0, (unsigned long) 0 );	

	    
	//
    // #importante:
    // Nessa hora podemos usar esse loop para pegarmos mensagens 
    // e enviarmos para o procedimento de janela do editor de texto.
    // Para assim tratarmos mensagens de mouse, para clicarmos e 
    // botões para salvarmos o arquivo. 
    // Devemos copiar a forma que foi feita o shell.
    //   

		while (running)
	    {
			//
			// #todo:
			// Isso é provisório, usaremos mensagens.
			// Aqui mesmo, nesse loop.
			//
			
			//enterCriticalSection(); 
	        ch = (int) getchar();
			//exitCriticalSection();
			
			if (ch == -1)
			{
			    asm ("pause");
               // printf("EOF reached! ?? \n");  				
			};
			
	        if (ch != -1)
	        {
	            printf ("%c", ch );
	    
	            //switch(ch)
                //{
			        //quit
			    //    case 'q':
			    //        goto hang;
				//        break;				 
		        //};		   
		    };
	    };		
		
		//saiu.
        printf(".\n");		
        printf(".\n");		
        printf(".\n");
		goto done;
	};
	
	
fail:	
    printf("fail.\n");
done:
    running = 0;
    printf("Exiting editor ...\n");
    printf("done.\n");
	while (1)
	{
		asm("pause");
		exit(0);
	};
    // Never reach this.	
	return (int) 0;
};


/*
 *  
 *     Limpar a tela 
 */

void editorClearScreen (){
	
	int lin, col;    

	// @todo:
	//system( "cls" ); // calls the cls command.
	
	//cursor.
	apiSetCursor ( 0, 0 );
	
	// Tamanho da tela. 80x25

	//linhas.
	for ( lin=0; lin < ((600/8)-1); lin++ )
	{
		col = 0;
		
		apiSetCursor(col,lin);
		
		//colunas.
		for ( col=0; col < ((800/8)-1); col++ ){
			
			printf("%c",' ');
	    }
	};
	
	apiSetCursor(0,2);
};




//testando a rotina de salvar um arquivo.
//estamos usando a API.
int editor_save_file (){
	
	//
	// #importante:
	// Não podemos chamar a API sem que todos os argumentos estejam corretos.
	//
	
	// #obs:
	// Vamos impor o limite de 4 setores por enquanto. 
	// 512*4 = 2048  (4 setores) 2KB
	// Se a quantidade de bytes for '0'. ???
	
	int Ret;
	
	char file_1[] = "Arquivo \n escrito \n em \n user mode no editor de textos teditor.bin :) \n";
	char file_1_name[] = "FILE2UM TXT";
	
	printf("editor_save_file: Salvando um arquivo ...\n");
	
	unsigned long number_of_sectors = 0;
    size_t len = 0;
	
	
	//
	// Lenght in bytes.
	//
	
	len = (size_t) strlen (file_1);

	if (len <= 0)
	{
	    printf ("editor_save_file:  Fail. Empty file.\n");
        return (int) 1;		
	}
	
	if (len > 2048)
	{
	    printf ("editor_save_file:  Limit Fail. The  file is too long.\n");
        return (int) 1;		
	}
	
    //
    // Number os sectors.
    //
	
	number_of_sectors = (unsigned long) ( len / 512 );
	
	if ( len > 0 && len < 512 ){
	    number_of_sectors = 1; 
    }		
	
	if ( number_of_sectors == 0 )
	{
	    printf ("editor_save_file:  Limit Fail. (0) sectors so save.\n");
        return (int) 1;				
	}
	
	//limite de teste.
	//Se tivermos que salvar mais que 4 setores.
	if ( number_of_sectors > 4 )
	{
	    printf ("editor_save_file:  Limit Fail. (%d) sectors so save.\n",
		    number_of_sectors );
        return (int) 1;				
	}
	
	
    Ret = (int) apiSaveFile ( file_1_name,  //name 
                              number_of_sectors,            //number of sectors.
                              len,            //size in bytes			
                              file_1,       //address
                              0x20 );       //flag		
				
	//if (Ret == 0)
	
	printf("t5: done.\n");	
	
	return (int) Ret;
};



