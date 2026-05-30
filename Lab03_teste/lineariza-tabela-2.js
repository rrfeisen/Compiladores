const tabela = document.getElementById("lrTableView")
const headtabela = tabela.getElementsByTagName("thead")
const headCelulas = headtabela[0].getElementsByTagName("tr")[2].getElementsByTagName("th")
const ACTION_GOTO_LIST = [];
for (var i = 0; i < headCelulas.length; i++) {
    ACTION_GOTO_LIST.push(headCelulas.item(i).textContent)
}
const bodytabela = tabela.getElementsByTagName("tbody")
const bodyLinhas = bodytabela[0].getElementsByTagName("tr")
const STATE_ACTION_GOTO_LINES = []
for (var j = 0; j < bodyLinhas.length; j++) {
    const linha = bodyLinhas.item(j)
    const initString = "" + j
    const celulasLinha = linha.getElementsByTagName("td")
    for (var k = 1; k < celulasLinha.length; k++) {
        const valueCelula = celulasLinha.item(k).textContent
        if (valueCelula === "\u00A0") continue
        const stringCelula = initString + " " + ACTION_GOTO_LIST[k - 1] + " " + valueCelula
        STATE_ACTION_GOTO_LINES.push(stringCelula)
    }
}

const TABLE_LINEARIZED = STATE_ACTION_GOTO_LINES.join('\n')

console.log(TABLE_LINEARIZED)

const conteudo = TABLE_LINEARIZED;
const blob = new Blob([conteudo], { type: 'text/plain' });
const link = document.createElement('a');
link.href = URL.createObjectURL(blob);
link.download = 'tabela_lr1.conf';
document.body.appendChild(link);
link.click();
document.body.removeChild(link);
