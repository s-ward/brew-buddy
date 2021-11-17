module.exports = {
  root: true,
  env: {
    node: true
  },
  'extends': [
    'plugin:vue/essential',
    '@vue/standard',
    'plugin:vue/base',
    'plugin:vuetify/base',
  ],
  rules: {
    'no-console': process.env.NODE_ENV === 'production' ? 'error' : 'off',
    'no-debugger': process.env.NODE_ENV === 'production' ? 'error' : 'off',
    'vue/valid-v-slot': 'off'
  },
  parserOptions: {
    parser: 'babel-eslint'
  }
}
