<?php

namespace App\Command;

use App\Service\BlobReferenceService;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;

class ProcessBlobReferencesCommand extends Command
{
    protected static $defaultName = 'app:check-blob-references';
    
    private $blobReferenceService;

    public function __construct(BlobReferenceService $blobReferenceService)
    {
        $this->blobReferenceService = $blobReferenceService;
        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setDescription('Checks the consistency of blob references.');
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $io = new SymfonyStyle($input, $output);

        // Run the blob consistency check
        $this->blobReferenceService->checkBlobConsistency($io);

        return Command::SUCCESS;
    }
}
