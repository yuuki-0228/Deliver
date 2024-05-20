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


Material lambert1 {
 0.400000;0.400000;0.400000;1.000000;;
 0.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
}

Frame pizza_fire {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame pPlane1 {
  

  FrameTransformMatrix {
   0.393701,0.000000,0.000000,0.000000,0.000000,-0.000000,-0.393701,0.000000,0.000000,0.393701,-0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh pPlane1 {
   15;
   0.000000;2.075254;1.032718;,
   0.000000;1.303123;0.000000;,
   0.000000;2.606246;0.000000;,
   0.000000;1.008886;1.692238;,
   0.000000;1.416771;-1.071545;,
   0.000000;2.833541;-1.071545;,
   0.000000;0.000000;0.000000;,
   0.000000;0.000000;1.841061;,
   0.000000;0.000000;-1.071545;,
   0.000000;-1.303123;0.000000;,
   0.000000;-1.008886;1.692238;,
   0.000000;-1.416771;-1.071545;,
   0.000000;-2.606246;0.000000;,
   0.000000;-2.075254;1.032718;,
   0.000000;-2.833541;-1.071545;;
   16;
   3;0,1,2;,
   3;1,0,3;,
   3;2,4,5;,
   3;4,2,1;,
   3;3,6,1;,
   3;6,3,7;,
   3;1,8,4;,
   3;8,1,6;,
   3;7,9,6;,
   3;9,7,10;,
   3;6,11,8;,
   3;11,6,9;,
   3;10,12,9;,
   3;12,10,13;,
   3;9,14,11;,
   3;14,9,12;;

   MeshNormals {
    15;
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;,
    1.000000;0.000000;0.000000;;
    16;
    3;0,1,2;,
    3;1,0,3;,
    3;2,4,5;,
    3;4,2,1;,
    3;3,6,1;,
    3;6,3,7;,
    3;1,8,4;,
    3;8,1,6;,
    3;7,9,6;,
    3;9,7,10;,
    3;6,11,8;,
    3;11,6,9;,
    3;10,12,9;,
    3;12,10,13;,
    3;9,14,11;,
    3;14,9,12;;
   }

   MeshMaterialList {
    1;
    16;
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
    0,
    0,
    0,
    0,
    0;
    { lambert1 }
   }

   MeshTextureCoords c1 {
    15;
    0.000000;1.000000;,
    0.500000;0.750000;,
    0.500000;1.000000;,
    0.000000;0.750000;,
    1.000000;0.750000;,
    1.000000;1.000000;,
    0.500000;0.500000;,
    0.000000;0.500000;,
    1.000000;0.500000;,
    0.500000;0.250000;,
    0.000000;0.250000;,
    1.000000;0.250000;,
    0.500000;0.000000;,
    0.000000;0.000000;,
    1.000000;0.000000;;
   }
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { pPlane1 }

  AnimationKey S {
   1;
   2;
   0;3;0.393701,0.393701,0.393701;;,
   4800;3;0.393701,0.393701,0.393701;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;0.707107,0.707107,0.000000,0.000000;;,
   4800;4;0.707107,0.707107,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   4800;3;0.000000,0.000000,0.000000;;;
  }
 }
}