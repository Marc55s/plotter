{
    description = "A simple Nix flake with GCC and Make";

    inputs = {
        nixpkgs.url = "nixpkgs/nixos-unstable";
    };

    outputs = { self, nixpkgs }: 
        let
            system = "x86_64-linux";  # Adjust for your system architecture
            pkgs = import nixpkgs { inherit system; };
        in
            {
            # Define the devShell with gcc and make
            devShell.${system} = pkgs.mkShell {
                buildInputs = [ 
                    pkgs.raylib
                    pkgs.cmake
                    pkgs.gnumake
                    pkgs.glibc
                    pkgs.clang_19
                    #pkgs.clang-tools
                    pkgs.libGL
                    # X11 dependencies
                    pkgs.xorg.libX11
                    pkgs.xorg.libX11.dev
                    pkgs.xorg.libXcursor
                    pkgs.xorg.libXi
                    pkgs.xorg.libXinerama
                    pkgs.xorg.libXrandr 
                ];

                shellHook = ''
                    export CC=${pkgs.gcc}/bin/gcc
                    export RAYLIB_INCLUDE=${pkgs.raylib}/include
                    export RAYLIB_LIB=${pkgs.raylib}/lib
                    export CFLAGS="-I${pkgs.raylib}/include"
                    export LDFLAGS="-L${pkgs.raylib}/lib"
                    export C_INCLUDE_PATH=/nix/store/isxsfmzsxsh1xa3lyh66zkv8w0qziai8-clang-19.1.5-lib/lib/clang/19/include
                    export TEST_VAR=${pkgs.gcc}
                    echo "Welcome to the development shell with GCC, Make, CMake and Raylib!";
                '';
            };

            # Define a build routine (if applicable)
            defaultPackage.${system} = pkgs.stdenv.mkDerivation {
                pname = "plotter";
                version = "1.0.0";

                src = ./.;  # Use the current directory for your source code

                # Optional: Environment variables for Raylib paths
                shellHook = ''
                    export CC=${pkgs.gcc}/bin/gcc
                    export RAYLIB_INCLUDE=${pkgs.raylib}/include
                    export RAYLIB_LIB=${pkgs.raylib}/lib
                    export CFLAGS="-I${pkgs.raylib}/include"
                    export LDFLAGS="-L${pkgs.raylib}/lib"
                    export C_INCLUDE_PATH=/nix/store/isxsfmzsxsh1xa3lyh66zkv8w0qziai8-clang-19.1.5-lib/lib/clang/19/include
                '';

                buildInputs = [ pkgs.raylib pkgs.cmake pkgs.gcc pkgs.gnumake 
                    pkgs.libGL
                    # X11 dependencies
                    pkgs.xorg.libX11
                    pkgs.xorg.libX11.dev
                    pkgs.xorg.libXcursor
                    pkgs.xorg.libXi
                    pkgs.xorg.libXinerama
                    pkgs.xorg.libXrandr ];

                buildPhase = ''
                    cmake .
                    make
                '';
                installPhase = ''
                mkdir -p $out/bin
                cp plotter $out/bin/
                '';
            };

            # Define a run routine (if applicable)
            defaultApp.${system} = {
                type = "app";
                program = "${self.defaultPackage.${system}}/bin/plotter";
            };
        };
}
