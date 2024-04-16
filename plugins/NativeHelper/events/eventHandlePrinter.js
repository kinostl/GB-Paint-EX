const id = "PT_PRINT_BACKGROUND";
const groups = ["Plugin Pak", "Printer"];
const name = "Print Screen Background";

const fields = [
  {
    key: "__scriptTabs",
    type: "tabs",
    defaultValue: "save",
    values: {
      save: "On Error",
    },
  },
  {
    key: "true",
    label: "On Error",
    type: "events",
  },
];

const compile = (input, helpers) => {
  const {
    appendRaw,
    getNextLabel,
    _declareLocal,
    _addComment,
    _compilePath,
    _label,
    _addNL,
    _callNative
  } = helpers;

  _addComment("Print screen background");

  const printStatusRef = _declareLocal("print_status", 1, true);
  const printSuccesfulLabel = getNextLabel();

  _callNative('printImageToOverlay');

  appendRaw(`; Detect printer and print
VM_PRINTER_DETECT       ${printStatusRef}, 30
VM_PRINT_OVERLAY        ${printStatusRef}, 0, 18, 0
`);

  appendRaw(`; Check status for error code
VM_RPN
  .R_REF      ${printStatusRef}
  .R_INT8     0xF0
  .R_OPERATOR .B_AND
  .R_STOP
VM_IF_CONST             .EQ, .ARG0, 0, ${printSuccesfulLabel}$, 1

VM_POP 1
`);

  _addNL();
  _compilePath(input.true);
  _label(printSuccesfulLabel);
  _addNL();
};

module.exports = {
  id,
  name,
  groups,
  fields,
  compile,
};
