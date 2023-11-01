#ifndef GENERIC_TRIGGER
#define GENERIC_TRIGGER

class GenericTrigger {
    protected:
        bool m_Armed;
    public:
        virtual bool Test();
        virtual bool IsArmed();
        virtual bool Arm();
        virtual void Disarm();
};

#endif