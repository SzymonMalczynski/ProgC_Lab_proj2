/*
 * libs.h
 *
 *  Created on: Apr 15, 2019
 *      Author: root
 */

#ifndef LIBS_H_
#define LIBS_H_

struct Node {
	int id;
	unsigned char *datagram;
	struct Node *next;
	struct Node *prev;
};

void sendPacket ();


#endif /* LIBS_H_ */
