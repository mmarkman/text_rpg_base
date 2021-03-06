#include <map.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct map_s {

	char *name;

	char **theMap;

	size_t width;
	size_t height;

	size_t nameLen;

} map_s;

#define MAXNAMELEN 256

#define NAME(m) (m->name)
#define THEMAP(m) (m->theMap)
#define W(m) (m->width)
#define H(m) (m->height)
#define NAMELEN(m) (m->nameLen)

int mapZeroizeFree( map m ) ;

int mapLoad( map m, const char *fileName )
{
	size_t i = 0;

	FILE *mapFilePtr = NULL;

	char *newName = (char *) malloc(sizeof(char) * MAXNAMELEN);

	if( fileName == NULL ) {

		printf("%s:%d:mapLoad(): parameter fileName passed is NULL\n",
		       __FILE__, __LINE__ ) ;
		fflush(stdout);
		return -1;
	}

	if( m == NULL ) {
		
		printf("%s:%d:mapLoad(): parameter m passed is NULL\n",
		       __FILE__, __LINE__ ) ;
		fflush(stdout);
		return -1;
	}

	mapFilePtr = fopen( fileName, "r");

	if ( mapFilePtr == NULL ) {

		printf("%s:%d:mapLoad(): fopen failed\n",
		       __FILE__, __LINE__ ) ;
		fflush(stdout);
		return -1 ; 
	}
	
	/* Get name from map data file, malloc space in object for it */

	fscanf( mapFilePtr , "%s", newName ) ;

	i = strlen( newName ) ;

	NAME(m) = (char *) malloc(sizeof(char) * i ) ;

	strcpy( NAME(m), newName ) ;

	/* Get height and width from map data file */

	fscanf( mapFilePtr , "%lu\n%lu", & H(m), & W(m) ) ;

	/* Malloc double array for map length */

	THEMAP(m) = (char **) malloc( sizeof ( char * ) * H(m) ) ; /* rows */

	for (i = 0 ; i < H(m) ; i++ ) {
		THEMAP(m)[i] = malloc( sizeof ( char ) * W(m) ) ; /* cols */
 		fscanf( mapFilePtr, "%s" , THEMAP(m)[i] ) ;
	}

	fclose( mapFilePtr ) ;

	return 0 ;
}

int mapInit( map *m )
{
	int rc;

	if( m == NULL )
	{
		printf("ERROR:%s:%d:mapInit(): was passed a NULL ptr\n",
		       __FILE__, __LINE__);

		fflush(stdout);
		return -1;
	}

	*m = ( (map) malloc( sizeof( map_s ) ) ) ;

	rc = mapZeroize( *m ) ;

	if( rc != 0 ) {
		
		printf("ERROR:%s:%d:mapInit(): mapZeroize() failed m=%p\n",
		       __FILE__, __LINE__, (void *) *m);
	}

	return 0;
}

int mapIsInitted( map m ) 
{
	return ( ( m == NULL ) ? 0 : 1 );
}

int mapDelete( map m )
{
	int rc;

	if( m == NULL ) {
		printf("WARNING:%s:%d:mapDelete(): was passed a NULL ptr."
		       "doing nothing.\n",
		       __FILE__, __LINE__ ) ;
		fflush(stdout);
		return 1;
	}

	rc = mapZeroizeFree( m ) ;

	if( rc != 0 ) {
		
		printf("ERROR:%s:%d:mapDelete(): "
		       "mapFreeInternals failed rc=%d, m=%p \n",
		       __FILE__, __LINE__, rc, (void *) m ) ;
		fflush(stdout);
		return rc;
	}

	rc = mapZeroize( m ) ;

	if( rc != 0 ) {

		printf("ERROR:%s:%d:mapDelete(): "
		       "mapZeroize failed rc=%d, m=%p. \n",
		       __FILE__, __LINE__, rc, (void *) m ) ;
		return rc;
	}

	free( m ) ;

	return 0;
}

int mapZeroizeFree( map m )
{
	size_t i;

	if( m == NULL ) {

		printf("ERROR:%s:%d:mapFreeInternals(): "
		       "was passed a null ptr\n",
		       __FILE__, __LINE__) ;
		fflush(stdout);
		return -1;
	}

	free( NAME(m) ) ;

	NAMELEN(m) = 0 ;

	NAME(m) = NULL ;

	for( i = 0; i < H(m) ; ++i ) {
		free( THEMAP(m) [i] ) ;
	}

	free( THEMAP(m) ) ;

	THEMAP(m) = NULL ;

	H(m) = 0 ;

	W(m) = 0 ;

	return 0 ;
}

int mapZeroize( map m ) 
{
	if( m == NULL ) {

		printf("ERROR:%s:%d:mapZeroize(): was passed a null ptr\n",
		       __FILE__, __LINE__ ) ;
		fflush( stdout ) ;
		return -1 ;
	}

	W(m) = 0 ;
	H(m) = 0 ;

	NAMELEN(m) = 0 ;

	return 0;
}

int mapPrint( map m )
{
	size_t i;
	
	if( m == NULL ) {
		
		printf("ERROR:%s:%d:mapPrint(): passed a null pointer\n",
		       __FILE__, __LINE__ ) ;
		fflush(stdout);
		return -1;
	}

	printf("name: %s\n", NAME(m) ) ;

	printf("Height: %zd Width: %zd\n", H(m), W(m) ) ;

	for( i = 0; i < H(m) ; ++i ) {

		printf("%s\n", THEMAP(m)[i] ) ;
	}

	return 0;
}

int mapNameLen( map m, size_t *nameLen )
{
	if( m == NULL)
	{
		printf("%s:%d:mapNameLen(): passed a null ptr\n",
		       __FILE__, __LINE__);
		fflush(stdout);
		return -1;
	}

	*nameLen = NAMELEN(m);

	return 0;
}

int mapWidth( map m, size_t *w )
{
	if( m == NULL)
	{
		printf("%s:%d:mapNameLen(): passed a null ptr\n",
		       __FILE__, __LINE__);
		fflush(stdout);
		return -1;
	}

	*w = W(m);

	return 0;
}

int mapHeight( map m, size_t *h )
{
	if( m == NULL)
	{
		printf("%s:%d:mapNameLen(): passed a null ptr\n",
		       __FILE__, __LINE__);
		fflush(stdout);
		return -1;
	}

	*h = H(m);

	return 0;
}

int mapName( map m, char **str_ptr )
{
	if( m == NULL || str_ptr == NULL )
	{
		printf("%s:%d:mapNameLen(): passed a null ptr "
		       "m=%p, str_ptr=%p\n",
		       __FILE__, __LINE__, (void *) m, (void *) str_ptr ) ;
		fflush(stdout);
		return -1;
	}

	*str_ptr = malloc(sizeof(char) * NAMELEN(m) ) ;

	strcpy( *str_ptr, NAME(m) ) ;

	return 0;
}

int mapCopy( map dest, map src )
{
	if( src == NULL || dest == NULL)
	{
		printf("%s:%d:mapNameLen(): passed a null ptr: "
		       "dest=%p, src=%p\n",
		       __FILE__, __LINE__, (void *) dest, (void *) src );
		fflush(stdout);
		return -1;
	}

	

	strcpy( NAME(dest), NAME(src) );

	return 0;
}
