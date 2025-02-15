{ pkgs, self }:
pkgs.nixosTest {
  nodes.machine =
    { pkgs, ... }:
    {
      imports = [
        self.nixosModules.deadbeef-vgmstream
        (self.inputs.nixpkgs + "/nixos/tests/common/x11.nix")
        (self.inputs.nixpkgs + "/nixos/tests/common/user-account.nix")
      ];
      test-support.displayManager.auto.user = "alice";
      services.xserver.enable = true;
      environment.systemPackages = [
        (pkgs.deadbeef-with-plugins.override {
          plugins = [ pkgs.deadbeefPlugins.vgmstream ];
        })
      ];
      hardware.alsa = {
        enable = true;
        enableRecorder = true;
        defaultDevice.playback = "pcm.recorder";
      };
      systemd.services.audio-recorder = {
        script = "${pkgs.alsa-utils}/bin/arecord -Drecorder -fS16_LE -r48000 -c2 /tmp/record.wav";
      };
      system.stateVersion = "24.11";
    };
  name = "deadbeef-vgmstream-basic";
  testScript =
    { nodes, ... }:
    let
      user = nodes.machine.users.users.alice;
    in
    ''
      from contextlib import contextmanager

      # This helper code is based on nixpkgs Firefox test.
      @contextmanager
      def record_audio(machine: Machine):
          machine.systemctl("start audio-recorder")
          yield
          machine.systemctl("stop audio-recorder")

      def wait_for_sound(machine: Machine):
          machine.wait_for_file("/tmp/record.wav")
          while True:
              machine.execute("tail -c 2M /tmp/record.wav > /tmp/last")
              size = int(machine.succeed("stat -c '%s' /tmp/last").strip())
              status, output = machine.execute(
                  f"cmp -i 50 -n {size - 50} /tmp/last /dev/zero 2>&1"
              )
              if status == 1:
                  break
              machine.sleep(2)

      machine.wait_for_x()
      machine.wait_for_file("${user.home}/.Xauthority")
      machine.succeed("xauth merge ${user.home}/.Xauthority")

      with subtest("Wait until DeaDBeeF starts up"):
        with record_audio(machine):
          machine.copy_from_host("${./test.adx}", "/tmp/test.adx")
          machine.execute("su - alice -c 'xterm -e deadbeef /tmp/test.adx' >&2 &")
          machine.wait_for_window("DeaDBeeF")
          machine.sleep(1)
          wait_for_sound(machine)
    '';
}
