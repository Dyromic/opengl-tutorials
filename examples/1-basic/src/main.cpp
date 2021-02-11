#include <iostream>
#include <cstdint>

#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct triangle {

    GLuint vao = 0;
    GLuint vbo = 0;

    triangle(GLuint vao = 0, GLuint vbo = 0) noexcept : vao{vao}, vbo{vbo} {};
    
    triangle(const triangle&) noexcept = delete;
    triangle(triangle&& o) noexcept : vao{std::exchange(o.vao, 0)}, vbo{std::exchange(o.vbo, 0)} {}

    triangle& operator=(triangle&& o) noexcept {
        vao = std::exchange(o.vao, 0); 
        vbo = std::exchange(o.vbo, 0);
        return *this;
    }

    triangle& operator=(const triangle&) noexcept = delete;

    ~triangle() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    static triangle create() {

        GLuint vao = 0;
        GLuint vbo = 0;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        float vertices[] = {
            -1.0f, -1.0f, 0.0f, 
            0.0f, 1.0f, 0.0f, 
            1.0f, -1.0f, 0.0f, 
        };

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(float), std::data(vertices), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, NULL);

        return triangle{vao, vbo};

    }

    void draw() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    }

};

struct shader {
    private:

        GLuint ID;

    public:

        shader(GLuint ID = 0) noexcept : ID{ ID } {}

        static shader create() {
            return shader(glCreateProgram());
        }

        shader(const shader&) noexcept = delete;
        shader(shader&& o) noexcept : ID{std::exchange(o.ID, 0)} {}

        shader& operator=(const shader&) noexcept = delete;
        shader& operator=(shader&& o) noexcept {
            ID = std::exchange(o.ID, 0);
            return *this;
        }

        virtual ~shader() noexcept {
            glDeleteProgram(this->ID);
        }

        void attach(GLuint shaderType, const char* shaderCode) noexcept {
            GLuint shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, std::addressof(shaderCode), nullptr);
            glCompileShader(shader);

            int success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            };

            glAttachShader(this->ID, shader);
            glDeleteShader(shader);
        }

        void link(std::string_view fragmentOutputName) noexcept {

            glBindFragDataLocation(this->ID, 0, std::data(fragmentOutputName));
            glLinkProgram(this->ID);

            int success;
            glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

            if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(this->ID, 512, nullptr, infoLog);
                std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }

        }

        void use() noexcept {
            glUseProgram(this->ID);
        }

};

struct application final {
    private:

		GLFWwindow* window;

    public:

        constexpr static std::uint32_t width = 640;
        constexpr static std::uint32_t height = 480;

        constexpr static const char* vertex_shader_code = R"(

            #version 330 core

            layout(location = 0) in vec3 position;
            
            out vec3 pos;

            void main() {

                pos = position + vec3(0.1, 0, 0);
                gl_Position = vec4(pos, 1.0f);
                
            }

        )";

        constexpr static const char* fragment_shader_code = R"(

            #version 330 core

            in vec3 pos;
            out vec4 color;
            
            void main() {
                color = vec4(abs(pos), 1.0f);

            }

        )";        

        constexpr static const char* vertex_shader_code2 = R"(

            #version 330 core
            
            layout(location = 0) in vec3 position;
            
            out vec3 pos;

            void main() {

                pos = position - vec3(0.1, 0, 0);
                gl_Position = vec4(pos, 1.0f);
                
            }


        )";

        shader triangle_shader;
        shader triangle_shader2;
        triangle tr;

        application() noexcept {


        }

        ~application() noexcept {}

        void init() {

            init_glfw();
            init_opengl();
            compile_shader();

            tr = triangle::create();

        }

        std::int32_t run() {

            static float last = static_cast<float>(glfwGetTime());
			while (!glfwWindowShouldClose(window)) {

				float now = static_cast<float>(glfwGetTime());
				last = now;

                render();

				glfwSwapBuffers(window);
				glfwPollEvents();

			}

            return 0;

        }

        void render() {
            
		    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            triangle_shader.use();
            tr.draw();
            triangle_shader2.use();
            tr.draw();

        }


    private:

        void init_glfw() {

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

			window = glfwCreateWindow(width, height, "OpenGL Basic 1", nullptr, nullptr);
			if (window == nullptr) {
				std::cerr << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
			}
			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
				std::cerr << "Failed to initialize GLAD" << std::endl;
			}

			//init_callbacks(); 

        }

        void init_opengl() {

			glViewport(0, 0, width, height);

			glEnable(GL_BLEND);
			glBlendFunc(
				GL_SRC_ALPHA,
				GL_ONE_MINUS_SRC_ALPHA
			);

			glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

        }

        void compile_shader() {
			
            triangle_shader = shader::create();
            triangle_shader.attach(GL_VERTEX_SHADER, vertex_shader_code);
            triangle_shader.attach(GL_FRAGMENT_SHADER, fragment_shader_code);
            triangle_shader.link("color");

            triangle_shader2 = shader::create();
            triangle_shader2.attach(GL_VERTEX_SHADER, vertex_shader_code2);
            triangle_shader2.attach(GL_FRAGMENT_SHADER, fragment_shader_code);
            triangle_shader2.link("color");

        }


};


int main() {

    application app;
    app.init();
    return app.run();

}