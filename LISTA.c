/***************************************************************************
*  $MCI módulo de implementação: LIS  Lista duplamente encadeada
*
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#define LISTA_OWN
#include "LISTA.h"
#undef LISTA_OWN

/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

   typedef struct tagElemLista {

         void * pValor ;
               /* Ponteiro para o valor contido no elemento */

         struct tagElemLista * pAnt ;
               /* Ponteiro para o elemento predecessor */

         struct tagElemLista * pProx ;
               /* Ponteiro para o elemento sucessor */

   } tpElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeça de lista
*
*
***********************************************************************/

   typedef struct LIS_tagLista {

         tpElemLista * pOrigemLista ;
               /* Ponteiro para a origem da lista */

         tpElemLista * pFimLista ;
               /* Ponteiro para o final da lista */

         tpElemLista * pElemCorr ;
               /* Ponteiro para o elemento corrente da lista */

         int numElem ;
               /* Número de elementos da lista */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a função de destruição do valor contido em um elemento */

   } LIS_tpLista ;

/***** Protótipos das funções encapuladas no módulo *****/

   static void LiberarElemento( LIS_tppLista   pLista ,
                                tpElemLista  * pElem   ) ;

   static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                       void *       pValor  ) ;

   static void LimparCabeca( LIS_tppLista pLista ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  $FC Função: LIS  &Criar lista
*  ****/

   LIS_tpCondRet LIS_CriarLista( void   ( * ExcluirValor ) ( void * pDado ), LIS_tppLista *pLista )
   {

      *pLista = ( LIS_tpLista * ) malloc( sizeof( LIS_tpLista )) ;
      if ( *pLista == NULL )
      {
         return LIS_CondRetFaltouMemoria ;
      } /* if */

      LimparCabeca( *pLista ) ;

      (*pLista)->ExcluirValor = ExcluirValor ;

      return LIS_CondRetOK;

   } /* Fim função: LIS  &Criar lista */

/***************************************************************************
*
*  $FC Função: LIS  &Destruir lista
*  ****/

   LIS_tpCondRet LIS_DestruirLista( LIS_tppLista pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  $FC Função: LIS  &Esvaziar lista
*  ****/

   LIS_tpCondRet LIS_EsvaziarLista( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;
      tpElemLista * pProx ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      pElem = pLista->pOrigemLista ;
      while ( pElem != NULL )
      {
         pProx = pElem->pProx ;
         LiberarElemento( pLista , pElem ) ;
         pElem = pProx ;
      } /* while */

      LimparCabeca( pLista ) ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Esvaziar lista */

/***************************************************************************
*
*  $FC Função: LIS  &Inserir elemento antes
*  ****/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                           void * pValor        )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inserir antes */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento antes do elemento corrente */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pAnt != NULL )
            {
               pElem->pAnt  = pLista->pElemCorr->pAnt ;
               pLista->pElemCorr->pAnt->pProx = pElem ;
            } else
            {
               pLista->pOrigemLista = pElem ;
            } /* if */

            pElem->pProx = pLista->pElemCorr ;
            pLista->pElemCorr->pAnt = pElem ;
         } /* if */

         pLista->pElemCorr = pElem ;

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  $FC Função: LIS  &Inserir elemento após
*  ****/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        )
      
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Criar elemento a inserir após */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

      /* Encadear o elemento após o elemento */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pProx != NULL)
            {
               pElem->pProx  = pLista->pElemCorr->pProx ;
               pLista->pElemCorr->pProx->pAnt = pElem ;
            } else
            {
               pLista->pFimLista = pElem ;
            } /* if */

            pElem->pAnt = pLista->pElemCorr ;
            pLista->pElemCorr->pProx = pElem ;

         } /* if */
                  
         pLista->pElemCorr = pElem ;
                  
         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento após */

/***************************************************************************
*
*  $FC Função: LIS  &Excluir elemento
*  ****/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      pElem = pLista->pElemCorr ;

      /* Desencadeia à esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } else {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } /* if */

      /* Desencadeia à direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
         } /* if */

      LiberarElemento( pLista , pElem ) ;

      return LIS_CondRetOK ;

   } /* Fim função: LIS  &Excluir elemento */

/***************************************************************************
*
*  $FC Função: LIS  &Remover apenas um elemento
*  ****/

   LIS_tpCondRet LIS_RemoverElemento( LIS_tppLista pLista, LIS_tppLista *pCabeca )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      pElem = pLista->pElemCorr ;

      /* Desencadeia à esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } 
		 else 
		 {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } /* if */

      /* Desencadeia à direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } 
		 else
         {
            pLista->pFimLista = pElem->pAnt ;
         } /* if */

		 pLista->numElem-- ;
		 
		return LIS_InserirElementoAntes( *pCabeca, pElem->pValor ) ;

   } /* Fim função: LIS  &Remover elemento */


/***************************************************************************
*
*  $FC Função: LIS  &Obter referência para o valor contido no elemento
*  ****/

   LIS_tpCondRet LIS_ObterValor( LIS_tppLista pLista, void **pValor )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
        return LIS_CondRetListaVazia ;
      } /* if */

      *pValor = pLista->pElemCorr->pValor ;
	  
	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Obter referência para o valor contido no elemento */


/***************************************************************************
*
*  $FC Função: LIS  &Obter tamanho da lista
*  ****/

   LIS_tpCondRet LIS_ObterTam( LIS_tppLista pLista, int *pTamanho )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      *pTamanho = pLista->numElem;
	  
	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Obter tamanho da lista*/

/***************************************************************************
*
*  $FC Função: LIS  &Ir para o elemento inicial
*  ****/

   LIS_tpCondRet IrInicioLista( LIS_tppLista *pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      (*pLista)->pElemCorr = (*pLista)->pOrigemLista ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  $FC Função: LIS  &Ir para o elemento final
*  ****/

   LIS_tpCondRet IrFinalLista( LIS_tppLista *pLista )
   {

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      (*pLista)->pElemCorr = (*pLista)->pFimLista ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  $FC Função: LIS  &Avançar elemento
*  ****/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem )
   {

      int i ;

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista != NULL ) ;
      #endif

      /* Tratar lista vazia */

         if ( pLista->pElemCorr == NULL )
         {

            return LIS_CondRetListaVazia ;

         } /* fim ativa: Tratar lista vazia */

      /* Tratar Avançar para frente */

         if ( numElem > 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i > 0 ; i-- )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pProx ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pFimLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar Avançar para frente */

      /* Tratar Avançar para trás */

         else if ( numElem < 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i < 0 ; i++ )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pAnt ;
            } /* for */

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pOrigemLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar Avançar para trás */

      /* Tratar não Avançar */

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Avançar elemento */

/***************************************************************************
*
*  $FC Função: LIS  &Procurar elemento contendo valor
*  ****/

   LIS_tpCondRet LIS_ProcurarValor( int (* CompararValor) (void * pDado1, void * pDado2),
									LIS_tppLista pLista , void * pValor )
   {

      tpElemLista * pElem ;

      #ifdef _DEBUG
         assert( pLista  != NULL ) ;
      #endif

      if ( pLista->pElemCorr == NULL )
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      for ( pElem  = pLista->pOrigemLista ;
            pElem != NULL ;
            pElem  = pElem->pProx )
      {
         if ( CompararValor( pElem->pValor, pValor ) == 0 )
         {
            pLista->pElemCorr = pElem ;
            return LIS_CondRetOK ;
         } /* if */
      } /* for */

      return LIS_CondRetNaoAchou ;

   } /* Fim função: LIS  &Procurar elemento contendo valor */


/***********************************************************************
*
*  $FC Função: LIS  &Retirar lista de elementos 
*  ****/

   LIS_tpCondRet LIS_RetirarListaElem(LIS_tppLista pLista, LIS_tppLista *pCabecaCorr) 
   {

	   tpElemLista *pCorr, *p;
	   int nElemCorr = 1;

	   #ifdef _DEBUG
		   assert(pLista != NULL);
	   #endif

	   if (pLista->pElemCorr == NULL) /* Lista vazia */
	   {
		   return LIS_CondRetListaVazia;
	   } /* if */

	   for (p = pLista->pElemCorr; p->pProx != NULL; p = p->pProx) /* Contagem de elementos depois do elemento a ser retirado */
		   nElemCorr++;

	   pCorr = pLista->pElemCorr;
	   
	   if(pCorr->pAnt != NULL)
	   {
			pCorr->pAnt->pProx = NULL;	/* Desencadeia à direita */
	   
			pLista->pElemCorr = pCorr->pAnt;	
	   
			pLista->pFimLista = pLista->pElemCorr;
	   }
	   else
	   {
			pLista->pOrigemLista = NULL;
			pLista->pFimLista = NULL;
			pLista->pElemCorr = NULL;
	   }
	   
	   pLista->numElem -= nElemCorr;
		   
	   pCorr->pAnt = NULL;	/* Desencadeia à esquerda */
	    
	   if( LIS_CriarLista(pLista->ExcluirValor, pCabecaCorr) == LIS_CondRetFaltouMemoria )
		   return LIS_CondRetFaltouMemoria;
	   (*pCabecaCorr)->numElem = nElemCorr;
	   (*pCabecaCorr)->pOrigemLista = pCorr;
	   (*pCabecaCorr)->pFimLista = p;
	   (*pCabecaCorr)->pElemCorr = p;

	   return LIS_CondRetOK;
   }/* Fim função: LIS  &Retirar lista de elementos */


/***********************************************************************
*
*  $FC Função: LIS  &Concatenar lista
*  ****/

   LIS_tpCondRet LIS_ConcatenarLista(LIS_tppLista pPrincipal, LIS_tppLista pConcatenada)
   {
	   if (pConcatenada->numElem == 0)
		   return LIS_CondRetListaVazia;
	   if (pPrincipal->numElem == 0) {
		   pPrincipal->pOrigemLista = pConcatenada->pOrigemLista;
		   pPrincipal->pElemCorr = pConcatenada->pElemCorr;
		 }
	   else {
		   pPrincipal->pFimLista->pProx = pConcatenada->pOrigemLista;		/* O próximo elemento do final da principal é o primeiro da concatenada */
		   pConcatenada->pOrigemLista->pAnt = pPrincipal->pFimLista;		/* O anterior ao primeiro da concatenada é o final da principal */
	   }
	   pPrincipal->numElem += pConcatenada->numElem;					/* Número de elementos da principal é somado ao da concatenada */
	   pPrincipal->pFimLista = pConcatenada->pFimLista;					/* Fim da lista principal é o fim da lista conctatenada */
	   free(pConcatenada);
	   return LIS_CondRetOK;
   }

/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: LIS  -Liberar elemento da lista
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

   void LiberarElemento( LIS_tppLista   pLista ,
                         tpElemLista  * pElem   )
   {

      if ( ( pLista->ExcluirValor != NULL )
        && ( pElem->pValor != NULL        ))
      {
         pLista->ExcluirValor( pElem->pValor ) ;
      } /* if */

      free( pElem ) ;

      pLista->numElem-- ;

   } /* Fim função: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Função: LIS  -Criar o elemento
*
***********************************************************************/

   tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                void *       pValor  )
   {

      tpElemLista * pElem ;

      pElem = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
      if ( pElem == NULL )
      {
         return NULL ;
      } /* if */

      pElem->pValor = pValor ;
      pElem->pAnt   = NULL  ;
      pElem->pProx  = NULL  ;

      pLista->numElem ++ ;

      return pElem ;

   } /* Fim função: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Função: LIS  -Limpar a cabeça da lista
*
***********************************************************************/

   void LimparCabeca( LIS_tppLista pLista )
   {

      pLista->pOrigemLista = NULL ;
      pLista->pFimLista = NULL ;
      pLista->pElemCorr = NULL ;
      pLista->numElem   = 0 ;

   } /* Fim função: LIS  -Limpar a cabeça da lista */

/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/
