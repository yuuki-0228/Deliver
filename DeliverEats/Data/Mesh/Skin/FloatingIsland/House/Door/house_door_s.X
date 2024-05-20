xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}

template AnimTicksPerSecond {
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}

template Animation {
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}

template AnimationSet {
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}

template FloatKeys {
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}


Material house_door {
 1.000000;1.000000;1.000000;1.000000;;
 0.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "house_door_0922.jpg";
 }
}

Frame house_door {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame house_door1 {
  

  FrameTransformMatrix {
   0.393701,0.000000,0.000000,0.000000,0.000000,-0.000000,-0.393701,0.000000,0.000000,0.393701,-0.000000,0.000000,-14.173229,3.740157,-8.070866,1.000000;;
  }

  Mesh house_door1 {
   24;
   -0.299999;5.500000;-9.500001;,
   0.299999;5.500000;9.499999;,
   0.299999;5.500000;-9.500001;,
   -0.299999;5.500000;9.499999;,
   -0.299999;5.500000;9.499999;,
   0.299999;-5.500000;9.499999;,
   0.299999;5.500000;9.499999;,
   -0.299999;-5.500000;9.499999;,
   -0.299999;-5.500000;9.499999;,
   0.299999;-5.500000;-9.500001;,
   0.299999;-5.500000;9.499999;,
   -0.299999;-5.500000;-9.500001;,
   -0.299999;-5.500000;-9.500001;,
   0.299999;5.500000;-9.500001;,
   0.299999;-5.500000;-9.500001;,
   -0.299999;5.500000;-9.500001;,
   0.299999;5.500000;-9.500001;,
   0.299999;-5.500000;9.499999;,
   0.299999;-5.500000;-9.500001;,
   0.299999;5.500000;9.499999;,
   -0.299999;-5.500000;-9.500001;,
   -0.299999;5.500000;9.499999;,
   -0.299999;5.500000;-9.500001;,
   -0.299999;-5.500000;9.499999;;
   12;
   3;0,1,2;,
   3;1,0,3;,
   3;4,5,6;,
   3;5,4,7;,
   3;8,9,10;,
   3;9,8,11;,
   3;12,13,14;,
   3;13,12,15;,
   3;16,17,18;,
   3;17,16,19;,
   3;20,21,22;,
   3;21,20,23;;

   MeshNormals {
    24;
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;;
    12;
    3;0,1,2;,
    3;1,0,3;,
    3;4,5,6;,
    3;5,4,7;,
    3;8,9,10;,
    3;9,8,11;,
    3;12,13,14;,
    3;13,12,15;,
    3;16,17,18;,
    3;17,16,19;,
    3;20,21,22;,
    3;21,20,23;;
   }

   MeshMaterialList {
    1;
    12;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    { house_door }
   }

   MeshTextureCoords c1 {
    24;
    0.958956;0.083763;,
    0.400511;0.066128;,
    0.958956;0.066128;,
    0.400511;0.083763;,
    0.400511;0.083763;,
    0.077201;0.066128;,
    0.400511;0.066128;,
    0.077201;0.083763;,
    0.956852;0.043277;,
    0.398409;0.025642;,
    0.956852;0.025642;,
    0.398409;0.043277;,
    0.398409;0.043277;,
    0.075098;0.025642;,
    0.398409;0.025642;,
    0.075098;0.043277;,
    0.033730;0.961756;,
    0.510499;0.137929;,
    0.510500;0.961756;,
    0.033730;0.137929;,
    0.510500;0.961756;,
    0.033731;0.137929;,
    0.033730;0.961756;,
    0.510500;0.137929;;
   }

   XSkinMeshHeader {
    1;
    1;
    1;
   }

   SkinWeights {
    "door_bone";
    24;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23;
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000,
    1.000000;
    1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,5.500000,9.500001,1.000000;;
   }
  }
 }

 Frame door_bone {
  

  FrameTransformMatrix {
   0.393701,0.000000,0.000000,0.000000,0.000000,-0.000000,-0.393701,0.000000,0.000000,0.393701,-0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { door_bone }

  AnimationKey S {
   1;
   2;
   0;3;0.393701,0.393701,0.393701;;,
   9280;3;0.393701,0.393701,0.393701;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;0.707107,0.707107,0.000000,0.000000;;,
   9280;4;0.707107,0.707107,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   9280;3;0.000000,0.000000,0.000000;;;
  }
 }
}