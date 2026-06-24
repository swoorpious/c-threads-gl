// copyright 2026 swaroop.

#include "shader_utils.h"
#include "core.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

static char* loadFile(const char* path) {
    FILE* file = fopen(path, "r");
    fseek(file, 0L, SEEK_END);

    const int file_size = ftell(file);
    char* buffer = (char*)malloc(file_size + 1);

    rewind(file);
    fread(buffer, 1, file_size, file);
    fclose(file);

    buffer[file_size] = '\0';
    return buffer;
}

static void parseShaderGroup(ShaderGroup* grp) {
    if (grp->raw == NULL) {
        printf("[shader_utils] no raw data available to parse.\n");
        return;
    }

    typedef enum {
        None = -1,
        Vertex = 0,
        Fragment = 1,
    } ShaderType;
    ShaderType shaderType = None;

    char vert[8192] = {0};
    char frag[8192] = {0};
    size_t size_vert = 0, size_frag = 0;

    char* line = strtok(grp->raw, "\n");

    while (line) {
        if (strcmp(line, "#shader VERTEX") == 0) shaderType = Vertex;
        else if (strcmp(line, "#shader FRAGMENT") == 0) shaderType = Fragment;

        else {
            switch (shaderType) {
            case Vertex:
                size_vert += sprintf(vert + size_vert, "%s\n", line);
                break;

            case Fragment:
                size_frag += sprintf(frag + size_frag, "%s\n", line);
                break;

            default: break;
            }
        }

        line = strtok(NULL, "\n");
    }

    grp->vert = (char*)malloc(size_vert * sizeof(char) + 1);
    grp->vert = strcpy(grp->vert, vert);

    grp->frag = (char*)malloc(size_frag * sizeof(char) + 1);
    grp->frag = strcpy(grp->frag, frag);
}


static GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("Shader error: %s\n", log);
    }
    return shader;
}

void loadShader(const char* shaderPath, ShaderGroup* grp) {
    char* fileData = loadFile(shaderPath);
    grp->raw = fileData;
    parseShaderGroup(grp);

    grp->compiledVert = compileShader(GL_VERTEX_SHADER, grp->vert);
    grp->compiledFrag = compileShader(GL_FRAGMENT_SHADER, grp->frag);

    free(fileData);
    free(grp->vert);
    free(grp->frag);
}

GLuint createProgram(const ShaderGroup* grp) {
    const GLuint program = glCreateProgram();
    const GLuint shad[2] = {grp->compiledVert, grp->compiledFrag};

    for (int i = 0; i < 2; i++) {
        int ok = 0;
        glGetShaderiv(shad[i], GL_COMPILE_STATUS, &ok);

        if (ok)
            glAttachShader(program, shad[i]);
    }
    glLinkProgram(program);

    for (int i = 0; i < 2; i++)
        glDeleteShader(shad[i]);

    return program;
}
