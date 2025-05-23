h37710
s 00067/00000/00000
d D 1.1 01/01/29 16:26:32 sinan 1 0
c date and time created 01/01/29 16:26:32 by sinan
e
u
U
f e 0
t
T
I 1
#include "Queue.h"
#include "Exception.h"

template <class Etype>
const Queue<Etype> &
Queue<Etype>::operator=( const Queue<Etype> & Rhs )
{
    if( this == &Rhs )
        return Rhs;

    MakeEmpty( );
    if( Rhs.IsEmpty( ) )
        return *this;

    QueueNode *Ptr = new QueueNode( Rhs.Front->Element );
    QueueNode *RhsPtr = Rhs.Front->Next;

    Front = Ptr;
    while( RhsPtr != NULL )
    {
        Ptr->Next = new QueueNode( RhsPtr->Element );
        RhsPtr = RhsPtr->Next;
        Ptr = Ptr->Next;
    }

    Ptr->Next = NULL;   // This line is unnecssary, but is in for clarity
    Back = Ptr;

    return *this;
}

template <class Etype>
void
Queue<Etype>::Enqueue( const Etype & X )
{
    if( IsEmpty( ) )
        Back = Front = new QueueNode( X );
    else
        Back = Back->Next = new QueueNode( X );
}

template <class Etype>
void
Queue<Etype>::Dequeue( )
{
    EXCEPTION( IsEmpty( ), "Queue is empty" );

    QueueNode *Old = Front;
    Front = Front->Next;
    delete Old;
}

template <class Etype>
const Etype &
Queue<Etype>::GetFront( ) const
{
    EXCEPTION( IsEmpty( ), "Queue is empty" );
    return Front->Element;
}

template <class Etype>
void
Queue<Etype>::MakeEmpty( )
{
    while( !IsEmpty( ) )
        Dequeue( );
}
E 1
