

/*
int main(int argc, char** argv) {
  if (argc > 1) {
    const char* basepath = "models/";
    if (argc > 2) {
      basepath = argv[2];
    }
    assert(true == TestLoadObj(argv[1], basepath));
  } else {
    // assert(true == TestLoadObj("cornell_box.obj"));
    // assert(true == TestLoadObj("cube.obj"));
    assert(true == TestStreamLoadObj());
    assert(true ==
           TestLoadObj("models/catmark_torus_creases0.obj", "models/", false));
  }

  return 0;
}

*/
