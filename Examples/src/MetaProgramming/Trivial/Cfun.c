/*
 * Cfun.c
 *
 *  Created on: May 21, 2019
 *      Author: forma
 */
struct MyStruct
{
  int m_a;
  double m_b;
};

void fillMyStruct(struct MyStruct * s)
{
  s->m_a=3;
  s->m_b=7.0;
}


