const std = @import("std");
const raySdk = @import("deps/raylib/build.zig");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.

const PathList = std.ArrayList([]const u8);
fn globFolder(
    allocator: std.mem.Allocator,
    b: *std.Build,
    path: []const u8,
    extension: []const u8,
) !PathList {
    var result = PathList.init(allocator);
    const cwd = std.fs.cwd();
    const directory = try cwd.openDir(path, .{
        .iterate = true,
        .no_follow = true,
    });
    var walker = try directory.walk(allocator);
    while (try walker.next()) |entry| {
        switch (entry.kind) {
            .file => {
                std.debug.print("{s}\n", .{entry.path});
                if (std.mem.eql(
                    u8,
                    std.fs.path.extension(entry.path),
                    extension,
                )) {
                    const file_path = b.pathJoin(&.{ path, entry.path });
                    std.debug.print("path: {s}\n", .{file_path});
                    try result.append(file_path);
                }
            },
            else => {},
        }
    }
    return result;
}

pub fn build(b: *std.Build) !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const lua_binder = b.addStaticLibrary(.{
        .name = "lua_binder",
        // In this case the main source file is merely a path, however, in more
        // complicated build scripts, this could be a generated file.
        .root_source_file = b.path("zig-src/lua_binder.zig"),
        .target = target,
        .optimize = optimize,
    });

    ////const luaJIT_sources = try globFolder(allocator, "deps/luajit/src/", ".c");
    ////lib.addCSourceFiles(.{ .files = luaJIT_sources.items });

    lua_binder.addIncludePath(b.path("deps/luajit/src"));
    lua_binder.addIncludePath(b.path("src/"));
    lua_binder.addIncludePath(b.path("deps/raylib/src"));
    lua_binder.linkLibC();
    lua_binder.linkLibCpp();
    //lua_binder.addObjectFile(.{ .src_path = "deps/luajit/src/libluajit.a" });

    //// This declares intent for the library to be installed into the standard
    //// location when the user invokes the "install" step (the default step when
    //// running `zig build`).
    b.installArtifact(lua_binder);

    const exe = b.addExecutable(.{
        .name = "wonky",
        //.root_source_file = b.path("src/main.c"),
        .target = target,
        .optimize = optimize,
    });
    exe.linkLibC();
    exe.linkLibCpp();

    const raylib = try raySdk.addRaylib(b, target, optimize, .{});
    exe.addIncludePath(b.path("deps/raylib/src"));
    exe.linkLibrary(raylib);

    exe.addIncludePath(b.path("deps/luajit/src"));
    exe.addObjectFile(b.path("deps/luajit/src/libluajit.a"));

    //const json_c_files = try globFolder(allocator, b, "deps/json-c/", ".c");
    //exe.addIncludePath(.{ .src_path = "deps/json-c/" });
    //exe.addCSourceFiles(.{ .files = json_c_files.items });

    const main_sources = try globFolder(allocator, b, "src/", ".c");
    exe.addCSourceFiles(.{ .files = main_sources.items });
    exe.addIncludePath(b.path("src/"));

    exe.linkLibrary(lua_binder);

    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    b.installArtifact(exe);
    b.installDirectory(.{
        .source_dir = b.path("assets"),
        .install_dir = .bin,
        .install_subdir = "assets",
    });

    // This *creates* a Run step in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = b.addRunArtifact(exe);

    // By making the run step depend on the install step, it will be run from the
    // installation directory rather than directly from within the cache directory.
    // This is not necessary, however, if the application depends on other installed
    // files, this ensures they will be present and in the expected location.
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    // Creates a step for unit testing. This only builds the test executable
    // but does not run it.
    const lib_unit_tests = b.addTest(.{
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    const run_lib_unit_tests = b.addRunArtifact(lib_unit_tests);

    const exe_unit_tests = b.addTest(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    const run_exe_unit_tests = b.addRunArtifact(exe_unit_tests);

    // Similar to creating the run step earlier, this exposes a `test` step to
    // the `zig build --help` menu, providing a way for the user to request
    // running the unit tests.
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_lib_unit_tests.step);
    test_step.dependOn(&run_exe_unit_tests.step);
}
