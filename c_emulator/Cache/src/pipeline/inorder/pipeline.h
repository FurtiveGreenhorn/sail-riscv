#include "../pipeline.h"

class Retire { };
class WB : public SimplePipelineUnitMixin<WB, Retire> {
public:
    WB(Clock& clock, Retire& out) : 
        SimplePipelineUnitMixin<WB, Retire>(clock, out){}
    
};
class Mem : public SimplePipelineUnitMixin<Mem, WB> {
public:
    Mem(Clock& clock, WB& out) : 
        SimplePipelineUnitMixin<Mem, WB>(clock, out){}
    
    void clockTicked() {
        
    }
};

class Execute : public SimplePipelineUnitMixin<Execute, Mem> {
public:
    Execute(Clock& clock, Mem& out) : 
        SimplePipelineUnitMixin<Execute, Mem>(clock, out){}
    
    void clockTicked() {

    }
};
class Decode : public SimplePipelineUnitMixin<Decode, Execute> {
public:
    Decode(Clock& clock, Execute& out) : 
        SimplePipelineUnitMixin<Decode, Execute>(clock, out){}

};

class Fetch : public SimplePipelineUnitMixin<Fetch, Decode> {
public:
    Fetch(Clock& clock, Decode& out) : 
        SimplePipelineUnitMixin<Fetch, Decode>(clock, out){}
    
};