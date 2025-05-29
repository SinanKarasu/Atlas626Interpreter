### TODO.md

#### 🔧 Header File Cleanup – Trailing Whitespace
These files were detected with unnecessary trailing spaces (`\ $`):

```
AnalogResourceContext.h  
AtlasDLG.h  
AtlasParser.h  
DataBusResourceContext.h  
DevicePortNode.h  
Edge.h  
Equivalence.h  
ExceptionTypes.h  
ExecEnv.h  
InitList.h  
LabelAST.h  
NestedTokenStream.h  
NodeName.h  
NounsModifiersDimensions.h  
PathNodeList.h  
PointSourceNode.h  
Resource.h  
ResourceContextBASE.h  
ReverseMap.h  
SensorDevicePortNode.h  
Signal.h  
SignalActionAST.h  
SourceDevicePortNode.h  
SourceLoad.h  
Std.h  
UutConnectorNode.h  
Vertex.h  
```

**Options:**
- Use `sed -i '' -e 's/ \+$//'` to clean.
- Set up a git pre-commit hook to reject or auto-fix.
- Or ignore, and let BBEdit haunt you with that inverted ❓
