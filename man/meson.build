mandir = join_paths(get_option('prefix'), get_option('mandir'))
man8dir = join_paths(mandir, 'man8')

conf = configuration_data()
conf.set('version', meson.project_version())
conf.set('man_date', man_date)

manpage = configure_file(
     input: files('simple-psb-broker.8.in'),
     output: 'simple-psb-broker.8',
     configuration: conf)

install_man(
     manpage,
     install_dir: man8dir)
