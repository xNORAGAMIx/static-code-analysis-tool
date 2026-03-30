class A {};

class B : public virtual A {};
class C : public B {};
class D : public A, public C {}; // ❌ indirect mismatch